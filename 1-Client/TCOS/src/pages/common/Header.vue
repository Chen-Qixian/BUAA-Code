<template>
  <div>
    <div class="header">
      <div class="title">
        基于拓扑映射的并行应用通信性能优化系统
      </div>
      <div class="title-en">
        <span class="highlight">T</span>opology
        <span class="highlight">M</span>apping 
        Based Parallel Application
        <span class="highlight">C</span>ommunication
        <span class="highlight">O</span>ptimization
        <span class="highlight">S</span>ystem
      </div>
      <a-dropdown>
        <a class="ant-dropdown-link" @click="e => e.preventDefault()">
          <span v-if="cn">系统语言</span>
          <span v-else>language</span>
          <a-icon type="down" /> 
        </a>
        <a-menu slot="overlay">
          <a-menu-item @click="toCn">
            <img class="flag" src="../../assets/images/CN.png" alt="简体中文">简体中文
          </a-menu-item>
          <a-menu-item @click="toEn">
            <img class="flag" src="../../assets/images/GB.png" alt="English">English
          </a-menu-item>
        </a-menu>
      </a-dropdown>
    </div>
  </div>
</template>

<script>

export default {
  name: 'CommonHeader',
  props: {
    cn: Boolean
  },
  methods: {
    toCn () {
      this.$router.push('/')
    },
    toEn () {
      this.$router.push('/en')
    }
  }
}
</script>

<style lang="stylus">
  @import '~styles/variables'
  .ant-dropdown-link
    position absolute
    top 50%
    right 40px
    transform translateY(-50%)
    color $textColor
    font-size 18px
    &:hover
      color $activeTextColor
  .anticon-down
    font-size 18px !important
  .ant-dropdown-menu
    width 120px
    background #666
  .ant-dropdown-menu-item
    display flex
    justify-content flex-start
    align-items center
    color #eee
    &:hover
      color #333
    .flag
      height 16px
      width 18px
      margin-right 5px
  .header
    background $bgColor
    color $textColor
    padding-top 20px
    padding-bottom 20px
    padding-left $toLeft
    padding-right 20px
    border-bottom $borderStyle
    z-index 10
    position relative
    .title
      font-size 26px
      color $purple1
      font-weight bold
    .title-en
      font-size 15px
      .highlight
        font-weight bold
        color $red1
</style>
