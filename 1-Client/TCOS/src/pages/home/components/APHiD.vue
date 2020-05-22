<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg" @fileSucc="handleFileSucc"></body-file>
      <body-para @postPara="postPara">
        <div class="input-para">
          <span>请输入胖树层数：</span>
          <a-input-number v-model="n" />
        </div>
        <div class="input-para">
          <span>请输入节点度数：</span>
          <a-input-number v-model="m" />
        </div>
      </body-para>
    </div>
    <body-preview :method="title" v-if="showPreview"></body-preview>
  </div>
</template>

<script>
import BodyHeader from './subcomponents/BodyHeader'
import BodyPara from './subcomponents/BodyPara'
import BodyFile from './subcomponents/BodyFile'
import BodyPreview from './subcomponents/BodyPreview'
export default {
  name: 'BodyAphid',
  components: {
    BodyHeader,
    BodyPara,
    BodyFile,
    BodyPreview
  },
  props: {
    paraStatus: Boolean
  },
  data() {
    return {
      title: 'APHiD',
      fullName: 'Algorithm for Placing Process Hierarchically on D-ary Trees',
      introduction: 'APHiD算法是一种向D叉锥形胖树（tapered fat-tree）网络结构上分层布局进程的算法，其目标在于将强通信的并行应用进程，分布在上述锥形胖树结构中相同或相邻的叶子交换机计算节点之间，从而达到优化通信性能的目的。',
      msg: '通信图文件上传',
      n: 3,
      m: 2,
      fileStatus: false
    }
  },
  computed: {
    showPreview: function() {
      return this.paraStatus && this.fileStatus
    }
  },
  methods: {
    handleFileSucc() {
      this.fileStatus = true
    },
    postPara () {
      this.$message.loading('提交中，请稍候', 1)
      let _this = this
      setTimeout(() => {
        if(parseInt(_this.m) == _this.m && parseInt(_this.n) == _this.n){
          let para = {
            n: _this.n,
            m: _this.m
          }
          _this.$emit('postPara', para)
          _this.$message.success('参数配置成功',2);
        }
        else if(!_this.m || !_this.n){
          _this.$emit('postPara', false)
          _this.$message.error('参数不能为空',2);
        }
        else {
          _this.$emit('postPara', false)
          _this.$message.error('参数必须为整数',2);
        }
      }, 1100);
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.wrapper >>> .ant-input-number-input
  background-color $activeBgColor
  color $activeTextColor
.wrapper
  padding 30px
  padding-bottom 10px
  .data
    width 450px
    float left
  .input-para
    color $activeTextColor
    margin 10px 0
</style>
