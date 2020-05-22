<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg" @fileSucc="handleFileSucc"></body-file>
      <body-para @postPara="postPara">
        <div class="input-para">
          <span>Calculation Nodes：</span>
          <a-input-number v-model="n" />
        </div>
        <div class="input-para">
          <span>Cores per Node：</span>
          <a-input placeholder="split data with space" allowClear v-model="m"/>
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
      introduction: 'The QTLS algorithm is suitable for computing the network structure with the feature of adjacency. Based on the fast community partition algorithm of QTLS, the iteration strategy and convergence criterion are designed, and the ideal effect is achieved.',
      msg: 'Upload Communication Topology',
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
      this.$message.loading('Submitting, please wait', 1)
      let _this = this
      let regex = /^\s*(\d+\s*)+$/
      setTimeout(() => {
        if(parseInt(_this.n) !== _this.n){
          _this.$emit('postPara', false)
          _this.$message.error('Parameter MUST be an integer', 2)
        }
        else if(!_this.n){
          _this.$emit('postPara', false)
          _this.$message.error('Parameter CANNOT be null',2);
        }
        else if(regex.test(_this.m) === false) {
          _this.$emit('postPara', false)
          _this.$message.error('Parameter format error', 2)
        }
        else {
          let para = {
            n: _this.n,
            m: _this.m
          }
          _this.$emit('postPara', para)
          _this.$message.success('Parameter Configure Successfully', 2);
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
