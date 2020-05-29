<template>
  <v-chart :options="radar"/>
</template>

<script>
import ECharts from 'vue-echarts'
import 'echarts/lib/chart/radar'
import 'echarts/lib/component/title'
export default {
  name: 'PreviewRadar',
  props: {
    method: String
  },
  components: {
    'v-chart': ECharts
  },
  data () {
    let dataSet = this.$store.state.response.previewRadarData
    let lineStyle = {
      normal: {
        width: 1,
        opacity: 0.9
      }
    }

    return {
      radar: {
        title: {
          text: '算法推荐程度排序',
          show: true,
          textStyle: {
            color: '#ccc',
            fontSize: 10
          },
          bottom: 0,
          left: 'center'
        },
        radar: {
          indicator: [
            {name: 'LPMS', max: 6},
            {name: 'QTMS', max: 6},
            {name: 'TOPO', max: 6},
            {name: 'APHID', max: 6},
            {name: 'TREE', max: 6},
            {name: 'MPIPP', max: 6}
          ],
          shape: 'circle',
          splitNumber: 6,
          name: {
            textStyle: {
              color: 'rgb(238, 197, 102)'
            }
          },
          splitLine: {
            lineStyle: {
              color: [
                'rgba(238, 197, 102, 0.1)',
                'rgba(238, 197, 102, 0.2)', 'rgba(238, 197, 102, 0.3)',
                'rgba(238, 197, 102, 0.5)', 'rgba(238, 197, 102, 0.6)',
                'rgba(238, 197, 102, 0.8)', 'rgba(238, 197, 102, 1)'
              ].reverse()
            }
          },
          splitArea: {
            show: false
          },
          axisLine: {
            lineStyle: {
              color: 'rgba(238, 197, 102, 0.5)'
            }
          },
          radius: '55%',
        },
        series: [
          {
            name: '效果对比',
            type: 'radar',
            lineStyle: lineStyle,
            data: dataSet,
            symbol: 'none',
            itemStyle: {
                color: '#F9713C'
            },
            areaStyle: {
                opacity: 0.6
            }
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
