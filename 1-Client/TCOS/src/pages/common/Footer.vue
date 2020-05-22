<template>
  <div class="footer">
    Copyright ©2019-2020. Chen Qixian, School of Computer Science and Engineering, Beijing University of Aeronautics and Astronautics. All rights reserved.
  </div>
</template>

<script>
export default {
  name: 'CommonFooter'
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.footer
  background $bgColor
  color $textColor
  padding-top 20px
  padding-bottom 20px
  padding-left $toLeft
  padding-right 20px
  border-top $borderStyle
  z-index 10
  position relative
  text-align center
  font-size 16px
</style>
