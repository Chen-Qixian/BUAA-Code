<template>
  <div class="nav">
    <div class="developer">
      <img
        src="@/assets/images/avatar.jpg"
        alt="avatar"
        class="avatar"
        @click="info"
      >
      <div class="info" v-if="cn">
        <p class="title" @click="info">开发单位</p>
        <p class="name" @click="info">北京航空航天大学</p>
        <p class="school" @click="info">计算机学院</p>
      </div>
      <div class="info" v-else>
        <p class="title" @click="info">Developed By</p>
        <p class="name" @click="info">Beihang University</p>
        <p class="school" @click="info">School of Computer Science and Engineering</p>
      </div>
    </div>
    <a-modal
      title="特别鸣谢"
      v-model="visible"
      @ok="handleOk"
      :footer="null"
      class="thanks"
      v-if="cn"
    >
      <p>本系统由 北京航空航天大学 计算机学院 陈麒先 开发与维护.</p>
      <p>在此向为完成本系统提供帮助的人士表示衷心的感谢!</p>
      <p>感谢疫情期间在家完成毕设过程中精心照顾和关爱我的父母和家人;</p>
      <p>感谢我的母校北航、我的指导老师肖利民老师、以及闫柏成博士;</p>
      <p>最后要特别感谢祁令姿同学一直以来的陪伴与鼓励</p>
      <!-- <p>在此特别向为完成本系统提供无私帮助的 祁令姿同学 表示衷心的感谢</p>
      <p>感谢你的貌美如花 和 不离不弃;</p>
      <p>感谢你在我困难时对我的 支持与鼓励；</p>
      <p>感谢你 给了我一个可以用心 爱你的机会;</p> -->
      <p>愿未来我们可以一路相伴 共同成长 谨记千里共明月 随君一路北航～</p>
    </a-modal>
    <a-modal
      title="Acknowledgement"
      v-model="visible"
      @ok="handleOk"
      :footer="null"
      class="thanks"
      v-else
    >
      <p>This system is developed and maintained by Chen Qixian, school of Computer Science, Beijing University of Aeronautics and Astronautics.</p>
      <p>Here for the completion of the system to help people express heartfelt thanks!</p>
      <p>I would like to thank my parents and family who took good care of me during the completion of the project at home during the outbreak</p>
      <p>Thanks to my Alma Mater, Beihang University, my instructor,Prof. Xiao Limin, and Dr. Yan Baicheng</p>
      <p>Finally, special thanks to Qi Lingzi for her constant company and encouragement</p>
      <!-- <p>在此特别向为完成本系统提供无私帮助的 祁令姿同学 表示衷心的感谢</p>
      <p>感谢你的貌美如花 和 不离不弃;</p>
      <p>感谢你在我困难时对我的 支持与鼓励；</p>
      <p>感谢你 给了我一个可以用心 爱你的机会;</p> -->
      <p>May we can accompany all the way in the future and always remember that I LOVE YOU.</p>
    </a-modal>
    <ul>
      <li
        v-for="(item, index) of itemList"
        :key="index"
        :class="activeStyle(index)"
        @click="itemClick($event, index)"
      >
        <a-icon :type="item.type"/>
        <span class="item-msg">{{item.msg}}</span>
      </li>
      <slot></slot>
    </ul>
  </div>
</template>

<script>
import { Modal } from 'ant-design-vue';
export default {
  name: 'CommonNavbar',
  props: {
    itemList: Array,
    navId: Number,
    cn: Boolean
  },
  data () {
    return {
      visible: false
    } 
  },
  methods: {
    itemClick (e, index) {
      this.$emit('change', index)
    },
    activeStyle(index) {
      return (index === this.navId) ? 'active nav-item': 'nav-item'
    },
    info () {
      this.visible = true
    },
    handleOk() {
      this.visible = false
    }
  }
}
</script>

<style lang="stylus">
@import '~styles/variables'
.ant-modal-header
  background cornflowerblue
  padding-left 40px
  .ant-modal-title
    color antiquewhite
    font-size 20px
.ant-modal-body
  background aliceblue
  border-radius 3px
  padding 40px
.ant-modal-close-icon > svg
  color bisque
.nav
  float left
  width 280px
  background-color $bgColor
  .developer
    height 137px
    border-bottom 1px solid $borderColor
    color $textColor
    padding 20px 0 20px $toLeft
    box-sizing border-box
    position relative
    .avatar
      height 60px
      width 60px
      border-radius 50%
      background-color #fff
      position absolute
      left $toLeft
      top 50%
      transform translateY(-50%)
      cursor pointer
    .info
      margin-left 80px
      p
        margin-bottom 10px
        cursor pointer
      .title
        font-size 18px
        font-weight bold
        color $activeTextColor
      .name
        font-size 16px
        font-weight bold
  ul
    background-color $bgColor
    margin 0
    .nav-item
      color $textColor
      height $navItemHeight
      line-height $navItemHeight
      text-align left
      padding-left $toLeft
      font-size 16px
      list-style none
      box-sizing border-box
      &:hover
        color $purple1
        cursor pointer
        background-color $activeBgColor
        .item-msg
          color $textColor
      .item-msg
        margin-left 10px
        padding-left 20px
        padding-top 8px
        padding-bottom 8px
        border-left $borderStyle
    .active
      color $purple1
      background-color $activeBgColor
      border-left 3px solid $purple1
      &:hover
        .item-msg
          color $activeTextColor
      .item-msg
        color $activeTextColor
</style>
