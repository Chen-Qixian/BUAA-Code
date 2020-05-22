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
          File Upload
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
      introduction: 'MPIPP is an MPI process layout tool. Based on the k-way graph partition algorithm, the graph mapping algorithm improves the objective function and makes it suitable for solving the graph mapping problem.',
      msg1: 'Upload Communication Graph',
      msg2: 'Upload Network Graph',
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
      this.$message.loading('Topology uploading, please wait', 1)
      let _this = this
      setTimeout(() => {
        if(!_this.fileSucc1){
          _this.$emit('postPara', false)
          _this.$message.error('Communication Graph CANNOT be null',2);
        }
        else if(!_this.fileSucc2){
          _this.$emit('postPara', false)
          _this.$message.error('Network Graph CANNOT be null',2);
        }
        else {
          _this.showPreview = true
          let para = {}
          _this.$emit('postPara', para)
          _this.$message.success('Topology File Uploaded Successfully',2);
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
