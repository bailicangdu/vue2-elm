<template>
  <div>
    <!-- 调用head-top时，只能传递参数给slot，不支持添加元素 -->
    <!-- signin-up 传参数 dash转驼峰 -->
    <!-- HTML 中的 attribute 名是大小写不敏感的，所以浏览器会把所有大写字符解释为小写字符。
    camelCase (驼峰命名法) 的 prop 名需要使用其等价的 kebab-case (短横线分隔命名) 命名： -->
    <head-top signin-up="home">
      <!-- 匿名插槽 -->
      <!-- 如果没有定义插槽名 则匿名slot都会插入元素且使用 即如果没有指定插哪个插座 则电器把每个匿名的插座都插上 -->
      <div>header</div>
      <div>footer</div>

      <!-- 作用域插槽 -->
      <!-- 当被提供的内容只有默认插槽，组件的标签被当作插槽的模板来使用。
      这样我们就可以把 v-slot 直接用在组件上. 
      只要出现多个插槽，请始终为所有的插槽使用完整的基于 <template> 的语法-->
      <template v-slot:test="slotProps">
        <div>header</div>
        <div>footer</div>
        {{ slotProps.test }}
      </template>

      <!-- 具名插槽 对传递内容进行唯一性的标识 -->
      <!-- 具名插槽缩写 v-slot:header 可以被重写为 #header -->
      <span slot="logo" class="head_logo" @click="reload">ele.me</span>
    </head-top>
    <nav class="city_nav">
      <div class="city_tip">
        <span>当前定位城市：</span>
        <span>定位不准时，请在城市列表中选择</span>
      </div>
      <router-link :to="'/city/' + guessCityid" class="guess_city">
        <span>{{guessCity}}</span>
        <svg class="arrow_right">
          <use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#arrow-right" />
        </svg>
      </router-link>
    </nav>
    <section id="hot_city_container">
      <h4 class="city_title">热门城市</h4>
      <ul class="citylistul clear">
        <router-link
          tag="li"
          v-for="item in hotcity"
          :to="'/city/' + item.id"
          :key="item.id"
        >{{item.name}}</router-link>
      </ul>
    </section>
    <section class="group_city_container">
      <ul class="letter_classify">
        <li v-for="(value, key, index) in sortgroupcity" :key="key" class="letter_classify_li">
          <h4 class="city_title">
            {{key}}
            <span v-if="index == 0">（按字母排序）</span>
          </h4>
          <ul class="groupcity_name_container citylistul clear">
            <router-link
              tag="li"
              v-for="item in value"
              :to="'/city/' + item.id"
              :key="item.id"
              class="ellipsis"
            >{{item.name}}</router-link>
          </ul>
        </li>
      </ul>
    </section>
  </div>
</template>

<script>
import headTop from "../../components/header/head";
import { cityGuess, hotcity, groupcity } from "../../service/getData";

export default {
  data() {
    return {
      guessCity: "", //当前城市
      guessCityid: "", //当前城市id
      hotcity: [], //热门城市列表
      groupcity: {}, //所有城市列表
    };
  },

  mounted() {
    // 获取当前城市
    cityGuess().then((res) => {
      this.guessCity = res.name;
      this.guessCityid = res.id;
    });

    //获取热门城市
    hotcity().then((res) => {
      this.hotcity = res;
    });

    //获取所有城市
    groupcity().then((res) => {
      this.groupcity = res;
    });
  },

  components: {
    headTop,
  },

  computed: {
    //将获取的数据按照A-Z字母开头排序
    sortgroupcity() {
      let sortobj = {};
      for (let i = 65; i <= 90; i++) {
        if (this.groupcity[String.fromCharCode(i)]) {
          sortobj[String.fromCharCode(i)] = this.groupcity[
            String.fromCharCode(i)
          ];
        }
      }
      return sortobj;
    },
  },

  methods: {
    //点击图标刷新页面
    reload() {
      window.location.reload();
    },
  },
};
</script>

<style lang="scss" scoped>
@import "../../style/mixin";
.head_logo {
  left: 0.4rem;
  font-weight: 400;
  @include sc(0.7rem, #fff);
  @include wh(2.3rem, 0.7rem);
  @include ct;
//   height: 50px;  // 后定义覆盖先定义
}
.city_nav {
  padding-top: 2.35rem;
  border-top: 1px solid $bc;
  background-color: #fff;
  margin-bottom: 0.4rem;
  .city_tip {
    @include fj;
    line-height: 1.45rem;
    padding: 0 0.45rem;
    span:nth-of-type(1) {
      @include sc(0.55rem, #666);
    }
    span:nth-of-type(2) {
      font-weight: 900;
      @include sc(0.475rem, #9f9f9f);
    }
  }
  .guess_city {
    @include fj;
    align-items: center;
    height: 1.8rem;
    padding: 0 0.45rem;
    border-top: 1px solid $bc;
    border-bottom: 2px solid $bc;
    @include font(0.75rem, 1.8rem);
    span:nth-of-type(1) {
      color: $blue;
    }
    .arrow_right {
      @include wh(0.6rem, 0.6rem);
      fill: #999;
    }
  }
}
#hot_city_container {
  background-color: #fff;
  margin-bottom: 0.4rem;
}
.citylistul {
  li {
    float: left;
    text-align: center;
    color: $blue;
    border-bottom: 0.025rem solid $bc;
    border-right: 0.025rem solid $bc;
    @include wh(25%, 1.75rem);
    @include font(0.6rem, 1.75rem);
  }
  li:nth-of-type(4n) {
    border-right: none;
  }
}
.city_title {
  color: #666;
  font-weight: 400;
  text-indent: 0.45rem;
  border-top: 2px solid $bc;
  border-bottom: 1px solid $bc;
  @include font(0.55rem, 1.45rem, "Helvetica Neue");
  span {
    @include sc(0.475rem, #999);
  }
}

.letter_classify_li {
  margin-bottom: 0.4rem;
  background-color: #fff;
  border-bottom: 1px solid $bc;
  .groupcity_name_container {
    li {
      color: #666;
    }
  }
}
</style>
