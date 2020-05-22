<template>
  <div class="result-file">
    <result-header>
      文件下载
      <a-button 
        type="primary"
        icon="logout" 
        size="large"
        shape="round"
        ghost
        class="btn"
        @click="btnClick(0)"
      >
        返回结果预览
      </a-button>
    </result-header>
    <result-body>
      <div class="download">
        <div class="hint">
          您的数据结果文档已生成
        </div>
        <a
          :href="filePath" 
          download="result" 
          class="download-link"
        >
          <a-button 
            type="primary"
            icon="cloud-download" 
            size="large"
            ghost
            class="btn-download"
          >
            立即下载
          </a-button>
        </a>     
      </div>
      <div class="support">
        <div class="hint1">
          认为映射优化方案对您有帮助？
        </div>      
        <div class="qcode">
          <div class="pay">
            <div class="wechat-pay-hint">
              <a-icon type="wechat"/>
              使用微信扫一扫
            </div>
            <img src="../../../assets/images/wechat.jpg" alt="wechat" class="pay-img">
          </div>
          <div class="pay">
            <div class="alipay-hint">
              <a-icon type="alipay-circle"/>
              使用支付宝扫一扫
            </div>
            <img src="../../../assets/images/alipay.jpg" alt="alipay" class="pay-img">
          </div>
        </div>
        <div class="hint2">
          支持一下，请开发者喝奶茶吧～
        </div>
      </div>
    </result-body>
  </div>
</template>

<script>
import ResultHeader from './subcomponents/ResultHeader'
import ResultBody from './subcomponents/ResultBody'

export default {
  name: 'ResultFile',
  components: {
    ResultHeader,
    ResultBody
  },
  data () {
    let path = this.$store.state.response.downloadPath
    console.log('path = ', path)
    return {
      filePath: `../../../../static/resultFile/${path}`
    }
  },
  methods: {
    btnClick (nav) {
      this.$emit('change', nav)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.result-file >>> .ant-btn
  border 3px solid
  margin-right 5px
  font-size 14px
  color $textColor
.btn:hover
  color $blue
.download
  margin-top 10px
  width 100%
  display flex
  flex-direction column
  justify-content center
  align-items center
  .download-link
    width 100%
    display flex
    justify-content center
  .hint
    color $yellow
    font-size 20px
    font-weight bold
    margin-bottom 20px
  .btn-download
    width 60%
    height 60px
    color $blue
    font-size 20px
  .btn-download:hover
    color $pink
.support
  margin-top 80px
  width 100%
  display flex
  flex-direction column
  justify-content center
  align-items center
  .hint1
    color $red1
    font-size 18px
    font-weight bold
    margin-bottom 10px
  .hint2
    color $purple1
    font-size 16px
    margin-top 40px
  .qcode
    display flex
    flex-direction row
    justify-content space-around
    align-items center
    width 70%
    .pay
      height 200px
      width 200px
      .wechat-pay-hint
        text-align center
        color #62b900
        font-weight bold
        font-size 16px
        margin-bottom 5px
      .alipay-hint
        text-align center
        color #108ee9
        font-weight bold
        font-size 16px
        margin-bottom 5px 
      .pay-img
        height 100%
        width 100%
</style>
