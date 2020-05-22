<template>
  <div class="param">
    <div class="hint">
      <a-icon type="setting"/>
      算法参数配置
    </div>
    <slot></slot>
    <a-button 
      type="primary"
      icon="check" 
      size="large"
      ghost
      class="btn"
      @click="btnClick()"
    >
      提交参数
    </a-button>
  </div>
</template>

<script>
export default {
  name: 'BodyPara',
  methods: {
    btnClick () {
      this.$emit('postPara')
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.param
  margin-top 60px
  width 450px
  position relative
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 15px 0 5px 0
  .btn
    position absolute
    bottom 0
    right 40px
    color $textColor
    border 2px solid $textColor
    font-size 12px
    &:hover
      color $green
      border 2px solid $green
</style>
