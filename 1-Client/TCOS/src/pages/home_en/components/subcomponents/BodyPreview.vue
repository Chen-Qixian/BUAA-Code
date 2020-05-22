<template>
  <div class="preview">
    <div v-if="loading" class="loading">
      <a-spin tip="Algorithm Evaluating, please wait" size="large"/>
    </div>
    <transition>
      <div v-if="showResult">
        <div class="hint">
          <a-icon type="line-chart"/>
          Effect Comparison
        </div>
        <div class="charts">
          <div class="chart">
            <!-- <v-chart theme="dark" :options="polar"/> -->
            <preview-radar></preview-radar>
          </div>
          <div class="chart">
            <preview-bar></preview-bar>
          </div>
        </div>
        <div class="confirm">Decided to use algorithm {{method}}？</div>
        <a-button
          type="primary"
          shape="round"
          :icon="calcIcon"
          size="large"
          @click="routerLink"
        >
          Start Calculation！
        </a-button>
      </div>
    </transition>
  </div>
</template>

<script>
import PreviewRadar from './PreviewRadar'
import PreviewBar from './PreviewBar'

const delay = 500 

export default {
  name: 'BodyPreview',
  props: {
    method: String
  },
  components: {
    PreviewRadar,
    PreviewBar
  },
  data () {
    return {
      loading: true,
      calcIcon: 'rocket'
    }
  },
  computed: {
    showResult() {
      return !this.loading
    }
  },
  methods: {
    routerLink () {
      this.$message.loading('Calculating, please wait', 1)
      this.calcIcon = 'loading'
      let _this = this
      setTimeout(() => {
        _this.$router.push('/result')
      }, delay)
    }
  },
  mounted () {
    let _this = this
    setTimeout(() => {
      _this.loading = false
    }, delay)
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.preview >>> .ant-btn
  margin-top 30px
  margin-left 10px
  height 50px
  width 200px
  border none
  background-color $red1
.preview >>> .echarts
  width $chartWidth
  height $chartHeight
.preview >>> .ant-spin-text
  color $orange
  font-size 18px
  font-weight bold
.preview >>> .ant-spin-dot>i
  background-color $orange
.preview
  height 390px
  margin-left 450px
  margin-right 30px
  margin-bottom 0
  .loading
    display flex
    height 100%
    justify-content center
    align-items center
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 15px 0 5px 10px
  .charts
    display flex
    flex-wrap wrap
    justify-content space-around
    align-items space-between
    .chart
      height $chartHeight
      width $chartWidth
  .confirm
    color $purple1
    font-size 20px
    font-weight bold
    margin 10px 0 -10px 10px
.v-enter
  opacity 0
  transform translateY(50px)
.v-enter-active
  transition all 1s
</style>
