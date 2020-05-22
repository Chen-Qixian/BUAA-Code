<template>
  <div class="result-header">
    <slot></slot>
  </div>
</template>

<script>
export default {
  name: 'ResultHeader'
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
  .result-header
    // border 2px solid white
    box-sizing border-box
    padding 30px
    font-size 30px
    font-weight bold
    color $orange
    display flex
    align-items center
    justify-content space-between
</style>
