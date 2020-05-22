<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg" @fileSucc="handleFileSucc"></body-file>
      <body-para @postPara="postPara">
        <div class="input-para">
          <span>请输入进程组总数：</span>
          <a-input-number v-model="n" />
        </div>
        <div class="input-para">
          <span>请输入每组进程数：</span>
          <a-input placeholder="每组进程数请用空格分开" allowClear v-model="m"/>
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
  name: 'BodyQtls',
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
      title: 'QTLS',
      fullName: 'Quadratic Time Complexity Locality Detection Algorithm for Shared HPC Systems',
      introduction: 'QTLS算法适用于计算节点有邻近性特征的网络结构，约束条件为：每个进程组内进程数应与对应节点内计算核心数相等，且最终划分得到的进程组数应与计算节点总数相等。QTLS基于快速社区划分算法，对其迭代策略和收敛判别条件进行设计，达到了理想的实现效果。',
      msg: '通信拓扑文件上传',
      n: 1,
      m: '',
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
      let regex = /^\s*(\d+\s*)+$/
      setTimeout(() => {
        if(parseInt(_this.n) !== _this.n){
          _this.$emit('postPara', false)
          _this.$message.error('进程组数必须为整数', 2)
        }
        else if(!_this.n){
          _this.$emit('postPara', false)
          _this.$message.error('参数不能为空',2);
        }
        else if(regex.test(_this.m) === false) {
          _this.$emit('postPara', false)
          _this.$message.error('每组进程数格式错误', 2)
        }
        else {
          let para = {
            n: _this.n,
            m: _this.m
          }
          _this.$emit('postPara', para)
          _this.$message.success('参数配置成功', 2);
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
.wrapper >>> .ant-input-affix-wrapper
  width 160px
  .ant-input
    background-color $activeBgColor
    color $activeTextColor
    font-size 10px
.wrapper >>> .ant-input-clear-icon
  color $textColor
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
