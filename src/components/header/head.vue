<template>
  <header id="head_top">
    <!-- 插槽的理解 自组件定义了一个插座 父组件定义电器 即父向子传递 -->
    <!-- https://cn.vuejs.org/v2/guide/components-slots.html -->

    <!-- 匿名插槽 -->
    <slot></slot>
    <div>content</div>

    <!-- 作用域插槽 -->
    <!-- 组件有test属性，只有head-top组件可以访问到test，如果要home可以渲染 -->
    <!-- 为了让test在父级的插槽内容中可用，可以将test作为slot元素的一个attribute绑定上去 -->
    <!-- 绑定在slot元素上的attribute称为插槽prop -->
    <slot name="test" v-bind:test="test">{{ test }}</slot>

    <!-- slot 如果没有指定任何内容，则显示默认内容 -->
    <slot name="logo">Default</slot>

    <!-- 具名插槽 如果没有指定名称 就是默认slot -->
    <slot name="search"></slot>
    
    <section class="head_goback" v-if="goBack" @click="$router.go(-1)">
      <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1">
        <polyline points="12,18 4,9 12,0" style="fill:none;stroke:rgb(255,255,255);stroke-width:2" />
      </svg>
    </section>
    <router-link :to="userInfo? '/profile':'/login'" v-if="signinUp" class="head_login">
      <svg class="user_avatar" v-if="userInfo">
        <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#user" />
      </svg>
      <span class="login_span" v-else>登录|注册</span>
    </router-link>
    <section class="title_head ellipsis" v-if="headTitle">
      <span class="title_text">{{headTitle}}</span>
    </section>
    <slot name="edit"></slot>
    <slot name="msite-title"></slot>
    <slot name="changecity"></slot>
    <slot name="changeLogin"></slot>
  </header>
</template>

<script>
import { mapState, mapActions } from "vuex";
export default {
  data() {
    return {
      test: "作用域插槽",
    };
  },
  mounted() {
    //获取用户信息
    this.getUserInfo();
  },
  props: ["signinUp", "headTitle", "goBack"],
  computed: {
    ...mapState(["userInfo"]),
  },
  methods: {
    ...mapActions(["getUserInfo"]),
  },
};
</script>

<style lang="scss" scoped>
@import "../../style/mixin";

#head_top {
  background-color: $blue;
  position: fixed;
  z-index: 100;
  left: 0;
  top: 0;
  @include wh(100%, 1.95rem);
  overflow: hidden;
}
.head_goback {
  left: 0.4rem;
  @include wh(0.6rem, 1rem);
  line-height: 2.2rem;
  margin-left: 0.4rem;
}
.head_login {
  right: 0.55rem;
  @include sc(0.65rem, #fff);
  @include ct;
  .login_span {
    color: #fff;
  }
  .user_avatar {
    fill: #fff;
    @include wh(0.8rem, 0.8rem);
  }
}
.title_head {
  @include center;
  width: 50%;
  color: #fff;
  text-align: center;
  .title_text {
    @include sc(0.8rem, #fff);
    text-align: center;
    font-weight: bold;
  }
}
</style>
