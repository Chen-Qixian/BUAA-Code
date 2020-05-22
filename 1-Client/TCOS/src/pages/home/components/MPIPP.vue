<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg1" @fileSucc="file1Succ"></body-file>
      <body-file :message="msg2" @fileSucc="file2Succ">
        <a-button 
          type="primary"
          icon="check" 
          size="large"
          ghost
          class="btn"
          @click="btnClick()"
        >
          提交参数
        </a-button>
      </body-file>
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
  name: 'BodyMpipp',
  components: {
    BodyHeader,
    BodyPara,
    BodyFile,
    BodyPreview
  },
  data() {
    return {
      title: 'MPIPP',
      fullName: 'MPI Process Placement toolset',
      introduction: 'MPIPP是一种MPI进程布局工具，它主要由通信图获取工具、目标集群网络拓扑建模工具和优化的启发式图映射算法三个关键部分组成。其中实现的图映射算法在k-way图划分算法的基础上改进目标函数，使之适用于解决图映射问题。',
      msg1: '通信图文件上传',
      msg2: '拓扑图文件上传',
      fileSucc1: false,
      fileSucc2: false,
      showPreview: false
    }
  },
  methods: {
    file1Succ () {
      this.fileSucc1 = true
    },
    file2Succ () {
      this.fileSucc2 = true
    },
    btnClick () {
      this.$message.loading('拓扑文件上传中，请稍候', 1)
      let _this = this
      setTimeout(() => {
        if(!_this.fileSucc1){
          _this.$emit('postPara', false)
          _this.$message.error('通信矩阵文件不能为空',2);
        }
        else if(!_this.fileSucc2){
          _this.$emit('postPara', false)
          _this.$message.error('网络拓扑文件不能为空',2);
        }
        else {
          _this.showPreview = true
          let para = {}
          _this.$emit('postPara', para)
          _this.$message.success('拓扑文件上传成功',2);
        }
      }, 1100);
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.wrapper >>> .hint
  margin-top 5px
.wrapper >>> #dropzone
  margin-top 0
  width 75%
  font-size 18px
.wrapper
  padding 30px
  padding-bottom 10px
  .data
    width 450px
    float left
    position relative
    .btn
      position absolute
      bottom 0
      right 20px
      color $textColor
      border 2px solid $textColor
      font-size 12px
      &:hover
        color $green
        border 2px solid $green
</style>
