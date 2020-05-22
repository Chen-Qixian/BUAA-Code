<template>
    <div class="file">
      <div class="hint">
        <a-icon type="upload"/>
        {{message}}
      </div>
      <vue-dropzone ref="myVueDropzone" id="dropzone" :options="dropzoneOptions"></vue-dropzone>
      <slot></slot>
    </div>
</template>

<script>
import vue2Dropzone from 'vue2-dropzone'
import 'vue2-dropzone/dist/vue2Dropzone.min.css'
export default {
  name: 'BodyFile',
  components: {
    vueDropzone: vue2Dropzone
  },
  props: {
    message: String
  },
  data () {
    let _this = this
    return {
      dropzoneOptions: {
        url: '/api/file',
        thumbnailWidth: 50,
        dictDefaultMessage: 'Drag File or Click to Upload', // 设置默认的提示语句
        paramName: "topoFile", // 传到后台的参数名称
        init: function () {
          this.on("success", function (file, data) {
            _this.$emit('fileSucc')
          });
        }
      }
    }
  }
}
</script>

<style lang="stylus" scoped>
@import '~styles/variables'
.file >>> #dropzone
  height 170px
  max-width 400px
  border-radius 20px
  border 3px dashed #ccc
  background-color $bgColor
  color $activeTextColor
  font-size 22px
  margin-top 10px
  .dz-message
    margin 0
    line-height 130px
  .dz-preview
    height 103px
    width 103px
    border-radius 20px
.file
  width 400px
  .hint
    color $purple1
    font-weight bold
    font-size 16px
    margin 15px 0 5px 0
</style>
