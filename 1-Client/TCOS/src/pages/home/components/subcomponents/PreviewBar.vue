<template>
  <v-chart :options="barLine"/>
</template>

<script>
import ECharts from 'vue-echarts'
import 'echarts/lib/chart/bar'
import 'echarts/lib/chart/line'
import 'echarts/lib/component/legend'
import 'echarts/lib/component/title'
import 'echarts/lib/component/tooltip'

// import 'echarts/lib/chart'
export default {
  name: 'PreviewBar',
  props: {
    method: String
  },
  components: {
    'v-chart': ECharts
  },
  data () {
    var colors = ['#5793f3', '#d14a61', '#675bba']
    let dataSet = this.$store.state.response.previewBarData
    return {
      barLine: {
        title: {
          text: '算法执行开销与代价函数计算值',
          show: true,
          textStyle: {
            color: '#ccc',
            fontSize: 10
          },
          bottom: 0,
          left: 'center'
        },
        color: colors,
        tooltip: {
          trigger: 'axis',
          axisPointer: {
              type: 'cross'
          }
        },
        grid: {
          left: '12%',
          right: '12%'
        },
        toolbox: {
          feature: {
            dataView: {show: true, readOnly: false},
            restore: {show: true},
            saveAsImage: {show: true}
          }
        },
        legend: {
          data: ['时间开销', '代价函数', '综合'],
          textStyle: {
            color: '#ccc'
          },
          orient: 'horizontal',
          itemWidth: 12,
          itemHeight: 8,
          top: 0
        },
        xAxis: [
          {
            type: 'category',
            axisTick: {
                alignWithLabel: true
            },
            data: ['LPMS', 'QTLS', 'TOPO', 'APHiD', 'TREE', 'MPIPP'],
            axisLabel: {
              color: '#ccc',
              rotate: 45,
              fontSize: 10,
              margin: 6
            }
          }
        ],
        yAxis: [
          {
            type: 'value',
            name: '时间开销',
            min: 0,
            max: 50,
            position: 'right',
            axisLine: {
              lineStyle: {
                color: colors[0]
              }
            },
            axisLabel: {
              show: false
            }
          },
          {
            type: 'value',
            name: '代价函数',
            min: 0,
            max: 50,
            position: 'left',
            axisLine: {
              lineStyle: {
                color: colors[1]
              }
            },
            axisLabel: {
              show: false
            }
          },
          {
            type: 'value',
            name: '综合',
            min: 0,
            max: 100,
            position: 'right',
            offset: 80,
            axisLine: {
              lineStyle: {
                color: colors[2]
              }
            }
          }
        ],
        series: [
          {
            name: '时间开销',
            type: 'bar',
            data: dataSet.algorithm
          },
          {
            name: '代价函数',
            type: 'bar',
            yAxisIndex: 1,
            data: dataSet.communication
          },
          {
            name: '综合',
            type: 'line',
            yAxisIndex: 2,
            data: dataSet.overall
          }
        ]
      }   
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
</style>
