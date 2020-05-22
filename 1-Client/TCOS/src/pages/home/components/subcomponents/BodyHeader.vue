<template>
  <div class="body-header">
    <div class="title">{{title}}</div>
    <div class="fullname">{{fullName}}</div>
    <div class="hint">
      <a-icon type="search"/>
      <span v-if="algorithm">算法概述</span>
      <span v-else>系统简介</span>
    </div>
    <div class="info">{{intro}}</div>
  </div>
</template>

<script>
export default {
  name: 'BodyHeader',
  props: {
    title: String,
    fullName: String,
    intro: String,
    algorithm: {
      type: Boolean,
      default: true
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.body-header
  margin-top -10px
  .title
    font-size 30px
    font-weight bold
    color $red1
  .fullname
    font-size 14px
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 15px 0 5px 0
  .info
    color $activeTextColor
    font-size 14px
    text-indent 2em
    line-height 30px
    text-align justify
    padding-right 40px
</style>
