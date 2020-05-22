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
          <a-input-number v-model="m" />
        </div>
      </body-para>
    </div>
    <body-preview :method="title" v-if="showPreview"></body-preview>
  </div>
</template>

<script>
import BodyHeader from './subcomponents/BodyHeader'
import BodyFile from './subcomponents/BodyFile'
import BodyPara from './subcomponents/BodyPara'
import BodyPreview from './subcomponents/BodyPreview'
export default {
  name: 'BodyLpms',
  components: {
    BodyHeader,
    BodyPara,
    BodyPreview,
    BodyFile
  },
  props: {
    paraStatus: Boolean
  },
  data() {
    return {
      title: 'LPMS',
      fullName: 'Label Propagation based Process Mapping Algorithm for Shared HPC Systems',
      introduction: 'LPMS算法适用于计算节点无邻近性特征的网络结构，是一种在标签传播算法的基础上改进而来的算法。它保留了标签传播算法在稀疏图划分问题中可并行和时间复杂度低的优点，通过重新设计标签初始化策略、迭代方式以及算法终止条件，满足了进程组划分的约束条件。',
      msg: '通信拓扑文件上传',
      m: 3,
      n: 4,
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
