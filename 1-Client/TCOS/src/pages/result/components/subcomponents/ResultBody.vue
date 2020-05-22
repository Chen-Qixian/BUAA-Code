<template>
  <transition>
    <div class="result-body" v-if="show">
      <slot></slot>
    </div>
  </transition> 
</template>

<script>
export default {
  name: 'ResultBody',
  data () {
    return {
      show: false
    }
  },
  mounted () {
    this.show = true
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.result-body
  // border 2px solid white
  box-sizing border-box
  height 480px
  padding 0
  display flex
  justify-content flex-start
  align-items flex-start
  flex-direction column
  margin -10px 50px 30px 50px
.v-enter
  opacity 0
  transform translateY(50px)
.v-enter-active
  transition all 1s
</style>
