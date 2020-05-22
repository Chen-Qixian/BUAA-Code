<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg" @fileSucc="handleFileSucc"></body-file>
      <body-para @postPara="postPara">
        <div class="input-para">
          <span>Fat-tree layers：</span>
          <a-input-number v-model="n" />
        </div>
        <div class="input-para">
          <span>Node degrees：</span>
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
      introduction: 'Aphid Algorithm is a kind of algorithm which lays out the process layer by layer on the structure of tapered fat-tree, its aim is to apply the process with strong communication in parallel, the utility model is distributed among the same or adjacent leaf switch computing nodes in the cone-shaped fat tree structure to optimize the communication performance.',
      msg: 'Upload Communication Graph File',
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
      this.$message.loading('Submitting, please wait', 1)
      let _this = this
      setTimeout(() => {
        if(parseInt(_this.m) == _this.m && parseInt(_this.n) == _this.n){
          let para = {
            n: _this.n,
            m: _this.m
          }
          _this.$emit('postPara', para)
          _this.$message.success('parameter configured',2);
        }
        else if(!_this.m || !_this.n){
          _this.$emit('postPara', false)
          _this.$message.error('parameter CANNOT be null',2);
        }
        else {
          _this.$emit('postPara', false)
          _this.$message.error('parameter MUST be an integer',2);
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
