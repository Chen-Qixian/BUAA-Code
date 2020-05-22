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
          <a-input-number v-model="m" />
        </div>
        <div class="input-para">
          <span>Task List：</span>
          <a-select defaultValue="OO" style="width: 120px" @change="handleSelection">
            <a-select-option value="OO">OO</a-select-option>
            <a-select-option value="BFS">BFS</a-select-option>
            <a-select-option value="DFS">DFS</a-select-option>
            <a-select-option value="minE" disabled>min-Edge</a-select-option>
          </a-select>
        </div>
        <div class="input-para">
          <span>packNodeFirst？</span>
          <a-switch defaultChecked @change="handleSwitch">
            <a-icon type="check" slot="checkedChildren" />
            <a-icon type="close" slot="unCheckedChildren" />
          </a-switch>
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
  name: 'BodyTopomapping',
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
      title: 'TopoMapping',
      fullName: 'Automatic Topology Mapping of Diverse Large-scale Parallel Applications',
      introduction: 'Topomaping provides an automated topology mapping scheme that can be applied to any communication mode in parallel as well as in physical topology without user intervention. The core of topo mapping is based on the GREEDYMAP Algorithm, multi-thread computation, and finally unified evaluation to get the optimal mapping scheme.',
      msg: 'Upload Network & Communication Topology',
      n: 16,
      m: 32,
      taskMode: 'OO',
      packNodeFirst: true,
      fileStatus: false
    }
  },
  computed: {
    showPreview: function() {
      return this.paraStatus && this.fileStatus
    }
  },
  methods: {
    handleSelection (value) {
      this.taskMode = value // value
    },
    handleSwitch (value) {
      this.packNodeFirst = value
    },
    handleFileSucc() {
      this.fileStatus = true
    },
    postPara () {
      this.$message.loading('Submitting, please wait', 1)
      let _this = this
      setTimeout(() => {
        if(parseInt(_this.m) == _this.m && parseInt(_this.n) == _this.n){
          let para = {
            n: this.n,
            m: this.m,
            taskMode: this.taskMode,
            packNodeFirst: this.packNodeFirst
          }
          _this.$emit('postPara', para)
          _this.$message.success('Parameter Configured Successfully',2);
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
.wrapper >>> .ant-select-selection
  background-color $activeBgColor
  color $activeTextColor
  .ant-select-arrow
    color $activeTextColor
.wrapper >>> .ant-input-number-input
  background-color $activeBgColor
  color $activeTextColor
.wrapper >>> .param
  margin-top -5px
.wrapper >>> .hint
  margin-bottom 0
  margin-top 10px
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
