# 前言

vue2的发布后自己也研究了一段时间，奈何公司的技术栈是以react为主，没有机会好好利用vue2去做一个完整的项目。虽然写了几个demo，但和写一个完整的项目还是有很大差别的。于是自己想着用空余的时间写一个项目，选择了饿了么也只是因为经常用，熟悉它的布局。之前的饿了么官网是用angular写的，最近才发现原来这段时间改成了vue1，看来饿了么也入了vue的坑。

既然要写一个完整的项目，就要认真对待，除了付款其他所有功能都尽可能实现，包括登陆、注册、个人中心、搜索、购物车、下单等等，也包括所有我能注意到的细节也都一并做出来，所以这绝对算是一个比较大的项目。

项目不使用模拟数据，所有数据均使用从官网实时获取的真实数据，最大程度实现和官网一样的功能，所以首先遇到的问题是跨域，我们启动本地服务器是获取不到官网数据的，这是跨域的。当然解决的方法很多，jsonp，nginx反向代理，webpack-dev-server的proxy，这里我用的是 http-proxy-middleware 原理都是一样的。


__注：此项目纯属个人瞎搞，正常下单请选择饿了么官方客户端。__


# 技术栈
vue2 + vue-rotuer2 + vuex2 + webpack + ES6/7 + fetch + sass + flex + svg + http-proxy-middleware反向代理 

# 目标功能
- [x] 定位功能 -- 完成
- [x] 选择城市 -- 完成
- [x] 搜索地址 -- 完成
- [x] 展示所选地址附近商家列表 -- 完成
- [x] 搜索美食，餐馆 -- 完成
- [x] 根据距离、销量、评分、特色菜、配送方式等进行排序和筛选 -- 完成
- [x] 餐馆详细页面 -- 完成
- [x] 购物车功能 -- 完成
- [x] 店铺评价页面 -- 完成
- [ ] 登陆、注册
- [ ] 修改密码
- [ ] 个人中心
- [ ] 帐户信息
- [ ] 服务中心
- [ ] 添加、删除、修改收货地址
- [ ] 订单列表
- [ ] 下单功能
- [ ] 付款(很难实现)


说明：因为并不是elm官方，而且因为要开代理，必须在pc端打开，所以预计最多只能做到下单这一步，下单成功后可以在手机客户端查看并付款。

# 项目布局
```
|-- build                            // webpack配置文件
|-- config                           // 项目打包路径
|-- elm                           	 // 上线项目文件，放在服务器即可正常访问
|
|-- src                              // 源码目录
|   |-- components                   // 组件
|       |-- common                   // 公共组件
|			|-- buyCart.js           // 购物车组件
|			|-- loading.js           // 页面初始化加载数据的动画组件
|			|-- mixin.js             // 组件混合(包括：指令-下拉加载更多，处理图片地址)
|			|-- ratingStar.js        // 评论的五颗星组件
|			|-- shoplist.js          // msite和shop页面的餐馆列表公共组件
|       |-- footer                   // 底部公共组件
|       |-- header                 	 // 头部公共组件
|
|   |-- config                       // 基本配置
|       |-- env.js                   // 环境切换配置
|       |-- fetch.js                 // 获取数据
|       |-- mUtils.js                // 常用的js方法
|       |-- rem.js                   // px转换rem
|
|   |-- images                       // 公共图片
|
|   |-- pages                        // 页面组件
|       |-- checkout                 // 确认订单页
|       |-- city                     // 当前城市页
|		|-- food                 	 // 特色餐馆列表页
|       |-- forget                   // 忘记密码，修改密码页
|       |-- home                     // 首页
|       |-- login                    // 登陆注册页
|       |-- msite                    // 商铺列表页
|       |-- order                    // 订单列表页
|       |-- profile                  // 个人中心
|       |-- search                   // 搜索页
|       |-- shop                     // 商铺筛选页
|			|-- children             
|				|-- rating           // 评论页   
|				|-- shopDetails      // 商铺信息页           
|       |-- vipcard                  // vip办理页
|
|   |-- plugins                      // 引用的插件
|
|   |-- router                       // 路由配置
|
|   |-- service                      // 数据交互统一调配
|		|-- template                 // 开发阶段的临时数据
|		|-- getData.js               // 获取数据的统一调配文件，对接口进行统一管理
|
|   |-- store                        // vuex的状态管理
|       |-- modules                  // store模块
|       |-- action.js                // 配置actions
|       |-- getters.js               // 配置getters
|       |-- index.js                 // 引用vuex，创建store
|       |-- mutation-types.js        // 定义常量muations名
|       |-- mutations.js             // 配置mutations
|
|   |-- style                        // 各种样式文件
|       |-- common.scss              // 公共样式文件
|       |-- mixin.scss               // 样式配置文件
|
|   |-- App.vue                      // 页面入口文件
|
|   |-- main.js                      // 程序入口文件，加载各种公共组件
|
|-- .babelrc                         // ES6语法编译配置
|-- .editorconfig                    // 代码编写规格
|-- .gitignore                       // 忽略的文件
|-- favicon.ico                      // 页面左上角小图标
|-- index.html                       // 入口html文件
|-- package.json                     // 项目及工具的依赖配置文件
|-- README.md                        // 说明
```



# 项目运行
```
克隆，或者直接下载
git clone https://github.com/bailicangdu/vue2-elm.git  

进入文件夹
cd vue2-elm

安装依赖
npm install
```

## 编译环境
```

开启本地服务器
npm run dev

访问 http://localhost:8088
```


## 线上版本
```
npm run build

生成的elm文件夹放在服务器即可正常访问
```


# 项目截图

## 城市列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/home.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/home.gif" width="365" height="619"/>


## 搜索地址页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/city.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/city.gif" width="365" height="619"/>


## 商铺列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/msite.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/msite.gif" width="365" height="619"/>


## 商铺筛选页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/food.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/food.gif" width="365" height="619"/>


## 搜索页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/search.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/search.gif" width="365" height="619"/>


## 餐馆详情页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/shop_cart.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/shop_cart.gif" width="365" height="619"/>

## 餐馆评论页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/rating.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/src/images/rating.gif" width="365" height="619"/>




如果觉得不错，请star一下吧 😊
