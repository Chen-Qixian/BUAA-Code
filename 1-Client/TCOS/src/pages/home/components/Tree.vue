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
          上传文件
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
  name: 'BodyTreematch',
  components: {
    BodyHeader,
    BodyPara,
    BodyFile,
    BodyPreview
  },
  data() {
    return {
      title: 'TreeMatch',
      fullName: 'Tree Structure Network Topology Matching Algorithm',
      introduction: 'TreeMatch算法主要包括三个步骤:收集并行应用通信特征信息，建模硬件结构特征，计算布局方式并为目标计算核心绑定特定进程。TreeMatch算法对于树形结构的网络拓扑，有着极佳的性能表现。',
      msg1: '通信矩阵上传',
      msg2: '树形拓扑上传',
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
      }, 1100)
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
