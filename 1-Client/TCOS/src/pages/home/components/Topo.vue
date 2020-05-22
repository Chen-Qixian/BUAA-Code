<template>
  <div class="wrapper">
    <body-header :title="title" :fullName="fullName" :intro="introduction"></body-header>
    <div class="data">
      <body-file :message="msg" @fileSucc="handleFileSucc"></body-file>
      <body-para @postPara="postPara">
        <div class="input-para">
          <span>请输入计算节点数：</span>
          <a-input-number v-model="n" />
        </div>
        <div class="input-para">
          <span>请输入计算核心数：</span>
          <a-input-number v-model="m" />
        </div>
        <div class="input-para">
          <span>请输入任务生成序：</span>
          <a-select defaultValue="OO" style="width: 120px" @change="handleSelection">
            <a-select-option value="OO">原始序列</a-select-option>
            <a-select-option value="BFS">广度优先</a-select-option>
            <a-select-option value="DFS">深度优先</a-select-option>
            <a-select-option value="minE" disabled>最小边缘切割</a-select-option>
          </a-select>
        </div>
        <div class="input-para">
          <span>是否使用节点优先分配？</span>
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
      introduction: 'TopoMaping算法提供了一种适用于任何通信模式的并行应用以及物理拓扑，且无需用户干预的自动化拓扑映射方案。TopoMapping的核心是基于GreedyMap算法多线程计算最后再统一评价得出最优映射方案。',
      msg: '上传通信图和网络拓扑文件',
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
      this.$message.loading('提交中，请稍候', 1)
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
.wrapper >>> .ant-select-selection
  background-color $activeBgColor
  color $activeTextColor
  .ant-select-arrow
    color $activeTextColor
.wrapper >>> .ant-input-number-input
  background-color $activeBgColor
  color $activeTextColor
.wrapper >>> .param
  margin-top -10px
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
