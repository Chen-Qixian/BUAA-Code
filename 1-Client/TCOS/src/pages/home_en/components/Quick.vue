<template>
  <div class="wrapper">
    <body-header
      :title="title"
      :fullName="fullName"
      :intro="introduction"
      :algorithm=false
    >
    </body-header>
    <div class="hint">
      <a-icon type="thunderbolt"/>
      Simple Use
    </div>
    <div class="quick">
      <vue-dropzone ref="myVueDropzone" id="dropzone" :options="dropzoneOptions"></vue-dropzone>
      <div class="bottom">
        <div class="input-para">
          <span>Input Parameters Needed：</span>
          <a-input placeholder="split params with space" allowClear v-model="m"/>
        </div>
        <a
          :href="filePath" 
          download="result"
          v-if="showDownload"
        >
          <a-button 
            type="primary"
            shape="round"
            icon="cloud-download"
            ghost
            size="large"
            class="btn-download"
          >
            Download Now!
          </a-button>
        </a>
        <a-button
          type="primary"
          shape="round"
          :icon="calcIcon"
          size="large"
          @click="calc"
          v-else
        >
          Cauculate Now！
        </a-button>
      </div>
    </div>
    <div class="hint" style="margin-top: -20px">
      <a-icon type="arrow-left"/>
      You can select a specific algorithm for the calculation in the left navigation bar
    </div>
  </div>
</template>

<script>
import BodyHeader from './subcomponents/BodyHeader'
import vue2Dropzone from 'vue2-dropzone'
import 'vue2-dropzone/dist/vue2Dropzone.min.css'

export default {
  name: 'BodyQuick',
  components: {
    BodyHeader,
    vueDropzone: vue2Dropzone
  },
  data () {
    let _this = this
    let path = this.$store.state.response.downloadPath
    return {
      title: 'Quick Start',
      fullName: 'Quick Start',
      introduction: 'The main principle of using topology mapping algorithm to optimize communication performance is to divide the process with large amount of data communication into the same process group, and then arrange it in the computing node with strong data transmission ability. In order to enable ordinary users of parallel applications to quickly and intuitively perceive the best topology mapping algorithm for a given scenario, the system integrates the six algorithms for users to choose and use, and combines the corresponding algorithm recommendation strategy, the efficiency of performance optimization of parallel application communication is greatly improved.',
      dropzoneOptions: {
        url: '/api/file',
        thumbnailWidth: 50,
        dictDefaultMessage: 'Drag File Here or Click to Upload', // 设置默认的提示语句
        paramName: "topoFile", // 传到后台的参数名称
        init: function () {
          this.on("success", function (file, data) {
            _this.fileStatus = true
          });
        }
      },
      m: '',
      calcIcon: 'rocket',
      fileStatus: false,
      showDownload: false,
      filePath: `../../../../static/resultFile/${path}`
    }
  },
  methods: {
    calc () {
      this.$message.loading('Calculating, please wait', 1)
      this.calcIcon = 'loading'
      let _this = this
      let regex = /^\s*(\d+\s*)+$/
      setTimeout(() => {
        _this.calcIcon = 'rocket'
        if(_this.fileStatus === false) {
          _this.$message.error('Please Upload Topology File', 2)
        }
        else if(regex.test(_this.m) === false) {
          _this.$message.error('Parameter format error', 2)
        }
        else {
          let para = { m: _this.m }
          _this.$emit('postPara', para)
          _this.$message.success('Click "Download Now" to get result', 2)
          _this.showDownload = true
        }
      }, 1000)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.wrapper >>> #dropzone
  height 170px
  width 100%
  border-radius 20px
  border 3px dashed #ccc
  background-color $bgColor
  color $activeTextColor
  font-size 22px
  margin-top 10px
  .dz-message
    margin 0
    line-height 130px
  .dz-preview
    height 103px
    width 103px
    border-radius 20px
.wrapper >>> .ant-input-affix-wrapper
  width 300px
  .ant-input
    background-color $activeBgColor
    color $activeTextColor
    font-size 10px
.wrapper >>> .ant-input-clear-icon
  color $textColor
.wrapper >>> .ant-btn
  margin-left 10px
  height 50px
  width 200px
  border none
  background-color $red1
  font-size 16px
.wrapper >>> .btn-download
  border 3px solid
  margin-right 5px
  font-size 16px
  color $red1
  &:hover
    color $blue
.wrapper
  padding 30px
  padding-bottom 10px
  display flex
  flex-direction column
  align-items flex-start
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 5px 0
  .quick
    overflow hidden
    width 95%
    .bottom
      display flex
      flex-direction row
      justify-content space-around
      align-items center
      .input-para
        color $activeTextColor
        margin 40px 0
</style>
