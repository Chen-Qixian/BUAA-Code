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
          <span>Cores per node：</span>
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
      introduction: 'Lpms is an improved algorithm based on tag propagation Algorithm. It retains the advantages of the label propagation algorithm in the sparse graph partitioning problem, such as parallelism and low time complexity, by re-designing the label initialization strategy, iterative method and the termination conditions of the algorithm, the constraint condition of process group partition is satisfied.',
      msg: 'Uploading Communication Topology',
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
      this.$message.loading('Submitting, please wait', 1)
      let _this = this
      setTimeout(() => {
        if(parseInt(_this.m) == _this.m && parseInt(_this.n) == _this.n){
          let para = {
            n: _this.n,
            m: _this.m
          }
          _this.$emit('postPara', para)
          _this.$message.success('Parameters configured successfully',2);
        }
        else if(!_this.m || !_this.n){
          _this.$emit('postPara', false)
          _this.$message.error('Parameter CANNOT be null',2);
        }
        else {
          _this.$emit('postPara', false)
          _this.$message.error('Parameter MUST be an integer',2);
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
