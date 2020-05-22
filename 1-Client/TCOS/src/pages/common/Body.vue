<template>
  <div class="body">
    <slot></slot>
  </div>
</template>

<script>

export default {
  name: 'CommonBody'
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.body
  margin-left 280px
  background-color $bgColor
  color $textColor
  height 620px
  border-left $borderStyle
  overflow hidden
</style>
