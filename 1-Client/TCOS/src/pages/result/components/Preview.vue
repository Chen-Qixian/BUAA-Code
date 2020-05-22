<template>
  <div class="result-preview">
    <result-header>
      结果预览
    </result-header>
    <result-body>
      <a-table
        :columns="columns"
        :dataSource="dataSet"
        :pagination="{ pageSize: 50 }"
        :scroll="{ y: 272 }"
      />
      <div class="choose">
        <div class="choose-item">
          <div class="hint">
            满意，下载数据文件！
          </div>
          <a-button 
            type="primary"
            icon="download" 
            size="large"
            ghost
            class="btn0"
            @click="btnClick(1)"
          >
          点我进入下载页
          </a-button>
        </div>
        <div class="choose-item">
          <div class="hint more">
            等等，我再看看！
          </div>
          <div class="btn-list">
            <a-button 
              type="primary"
              icon="apartment" 
              size="large"
              shape="round"
              ghost
              class="btn1"
              @click="btnClick(2)"
            >
              查看树形图
            </a-button>
            <a-button 
              type="primary"
              icon="dot-chart" 
              size="large"
              shape="round"
              ghost
              class="btn2"
              @click="btnClick(3)"
            >
              查看热力图
            </a-button>
            <a-button 
              type="primary"
              icon="swap" 
              size="large"
              shape="round"
              ghost
              class="btn3"
              @click="btnClick(4)"
            >
              查看映射图
            </a-button>
          </div>
        </div>   
      </div>
    </result-body>
  </div>
</template>

<script>
import ResultHeader from './subcomponents/ResultHeader'
import ResultBody from './subcomponents/ResultBody'

import { mapState } from 'vuex'

const columns = [
  {
    title: '计算节点序号',
    dataIndex: 'nodeId',
    width: 200,
    align: 'center'
  },
  {
    title: '计算核心数',
    dataIndex: 'coreNum',
    width: 200,
    align: 'center'
  },
  {
    title: '分配进程序列',
    dataIndex: 'process',
    width: 500,
    align: 'center',
    ellipsis: true
  },
]

export default {
  name: 'ResultPreview',
  components: {
    ResultHeader,
    ResultBody
  },
  data() {
    return {
      columns,
    }
  },
  computed: {
    ...mapState({
      res: 'response'
    }),
    dataSet() {
      return this.res.resultPreview
    }
  },
  methods: {
    btnClick (nav) {
      this.$emit('change', nav)
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.result-preview >>> .ant-table-header
  background $bgColor
.result-preview >>> .ant-table-align-center
  background $bgColor
  border $borderStyle
  color $orange
  font-size 16px
.result-preview >>> .ant-table-row
  background $bgColor
  border $borderStyle
  color $textColor
  td 
    border $borderStyle
  &:hover
    td
      background $activeBgColor
      color #eee
      font-weight bold
      cursor pointer
.result-preview >>> .ant-pagination-item-link
  background-color $bgColor
  color $activeTextColor
.result-preview >>> .ant-pagination-item
  background-color $bgColor
  a
    color $activeTextColor
.result-preview >>> .ant-btn
  border 3px solid
  margin-right 5px
  font-size 14px
  color $textColor
.choose
  // border 2px solid white
  width 100%
  display flex
  justify-content space-around
  .hint
    color $red1
    font-size 18px
    font-weight bold
    margin-bottom 10px
  .hint.more
    color $purple1
  .choose-item
    display flex
    flex-direction column
    justify-content center
    align-items center
    .btn0:hover
      color $red1
    .btn1:hover
      color $green
    .btn2:hover
      color $pink
    .btn3:hover
      color $yellow
</style>
