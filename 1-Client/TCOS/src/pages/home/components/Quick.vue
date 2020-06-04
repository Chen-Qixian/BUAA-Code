<template>
  <div class="wrapper">
    <body-header
      :title="title"
      :fullName="fullName"
      :intro="introduction"
      :algorithm=false
    >
    </body-header>
    <div class="hint">
      <a-icon type="thunderbolt"/>
      立即体验
    </div>
    <div class="quick">
      <vue-dropzone ref="myVueDropzone" id="dropzone" :options="dropzoneOptions"></vue-dropzone>
      <div class="bottom">
        <div class="input-para">
          <span>请输入算法所需参数：</span>
          <a-input placeholder="各参数请用空格分开" allowClear v-model="m"/>
        </div>
        <a
          :href="filePath" 
          download="result"
          v-if="showDownload"
        >
          <a-button 
            type="primary"
            shape="round"
            icon="cloud-download"
            ghost
            size="large"
            class="btn-download"
          >
            立即下载!
          </a-button>
        </a>
        <a-button
          type="primary"
          shape="round"
          :icon="calcIcon"
          size="large"
          @click="calc"
          v-else
        >
          开始计算！
        </a-button>
      </div>
    </div>
    <div class="hint" style="margin-top: -20px">
      <a-icon type="arrow-left"/>
      您可以在左侧导航栏中选择特定的算法进行计算
    </div>
  </div>
</template>

<script>
import BodyHeader from './subcomponents/BodyHeader'
import vue2Dropzone from 'vue2-dropzone'
import 'vue2-dropzone/dist/vue2Dropzone.min.css'

export default {
  name: 'BodyQuick',
  components: {
    BodyHeader,
    vueDropzone: vue2Dropzone
  },
  data () {
    let _this = this
    let path = this.$store.state.response.downloadPath
    return {
      title: '快速开始',
      fullName: 'Quick Start',
      introduction: '利用拓扑映射算法进行通信性能优化的主要原理是将存在大量数据通信的进程划分至相同进程组，进而将其布置在数据传输能力强的计算节点中。因此，并行应用的通信局部性特征成为了拓扑映射优化的重要考虑因素。本系统实现了LPMS、QTMS、TopoMapping、APHiD、MPIPP以及TreeMatch六种算法。为了让并行应用的普通使用者能够快速、直观地感知针对给定场景的最佳拓扑映射算法，本系统将这六种算法进行集成，供用户选择使用，结合相应的算法推荐策略，极大提高了并行应用通信性能优化的效率。',
      dropzoneOptions: {
        url: '/api/file',
        thumbnailWidth: 50,
        dictDefaultMessage: '拖动文件至此或者点击上传拓扑文件', // 设置默认的提示语句
        paramName: "topoFile", // 传到后台的参数名称
        init: function () {
          this.on("success", function (file, data) {
            _this.fileStatus = true
          });
        }
      },
      m: '',
      calcIcon: 'rocket',
      fileStatus: false,
      showDownload: false,
      filePath: `../../../../static/resultFile/output`
    }
  },
  methods: {
    calc () {
      this.$message.loading('拓扑映射方案计算中，请稍候', 1)
      this.calcIcon = 'loading'
      let _this = this
      let regex = /^\s*(\d+\s*)+$/
      setTimeout(() => {
        _this.calcIcon = 'rocket'
        if(_this.fileStatus === false) {
          _this.$message.error('请上传拓扑文件', 2)
        }
        else if(regex.test(_this.m) === false) {
          _this.$message.error('参数输入错误', 2)
        }
        else {
          let para = { m: _this.m }
          _this.$emit('postPara', para)
          _this.$message.success('计算方案已生成，点击立即下载', 2)
          _this.showDownload = true
        }
      }, 1000)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.wrapper >>> #dropzone
  height 170px
  width 100%
  border-radius 20px
  border 3px dashed #ccc
  background-color $bgColor
  color $activeTextColor
  font-size 22px
  margin-top 10px
  .dz-message
    margin 0
    line-height 130px
  .dz-preview
    height 103px
    width 103px
    border-radius 20px
.wrapper >>> .ant-input-affix-wrapper
  width 300px
  .ant-input
    background-color $activeBgColor
    color $activeTextColor
    font-size 10px
.wrapper >>> .ant-input-clear-icon
  color $textColor
.wrapper >>> .ant-btn
  margin-left 10px
  height 50px
  width 200px
  border none
  background-color $red1
  font-size 16px
.wrapper >>> .btn-download
  border 3px solid
  margin-right 5px
  font-size 16px
  color $red1
  &:hover
    color $blue
.wrapper
  padding 30px
  padding-bottom 10px
  display flex
  flex-direction column
  align-items flex-start
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 5px 0
  .quick
    overflow hidden
    width 95%
    .bottom
      display flex
      flex-direction row
      justify-content space-around
      align-items center
      .input-para
        color $activeTextColor
        margin 40px 0
</style>
