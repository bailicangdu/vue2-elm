## 前言

初学vue时曾在网上搜索vue的实战项目源码，无奈大部分都是简单的demo，对于深究vue没有太大的帮助，剩下的一些大部分都是像音乐播放器之类的展示型项目，交互没有预期那么复杂。但我们实际在工作中，经常会遇到有购物车的项目，这类项目因为涉及到money，所以对逻辑严谨度要求高，页面之间交互复杂，又会伴随着登陆、注册、用户信息等等，常常会让我们很头疼。既然还没人用vue写过这样的项目，那不如我来写，开源出来对能看到的人也会有帮助。

这种功能性的项目很实用但是往往也很枯燥，没有音乐播放器那么看起来绚丽，思来想去发现饿了么是一个不错的素材，一来它足够复杂，开放的外卖平台比一般的公司独有商店更加复杂。二来 见到那么多美食，大家也不会感觉到厌烦。

为啥是饿了么，而不是百度，美团？原因很简单，三个外卖大佬里，饿了么的色调和布局是最漂亮的，看起来最舒服。

此项目大大小小共 40 多个页面，涉及注册、登陆、商品展示、购物车、下单等等，是一个完整的流程。一般公司即便是官网的单页面项目都没这么复杂，如果这个项目能驾驭的了，相信大部分公司的其他单页面应用也就不在话下，即便更复杂，也不会比这个高到哪里去。

因为利用业余时间来做，年前就开始写，又跨个年，周期有点长，项目从零布局到完成共用了2个多月的时间，目前项目已经完成，正在进行一些性能的优化，增加详细的注释。

另外，这个项目和慕课网视频的那个饿了么没有任何关系，慕课网的项目只有一个页面，我在看完vue的官方文档后直接写了这个项目，没有参照任何人的代码，请大家不要混为一谈。

__注：此项目纯属个人瞎搞，正常下单请选择饿了么官方客户端。__





## 技术栈
vue2 + vuex + vue-router + webpack + ES6/7 + fetch + sass + flex + svg



## 项目运行（nodejs 6.0+）

```

git clone https://github.com/bailicangdu/vue2-elm.git  

cd vue2-elm

安装依赖
npm install
```

### 编译环境
```

npm run dev

访问 http://localhost:8088
```


### 线上版本
```
npm run build

生成的elm文件夹放在服务器即可正常访问
```




## 说明

>  本项目主要用于熟悉如何用 vue2 架构一个大型项目

>  如有问题请直接在 Issues 中提，或者可以加我的QQ 1264889788

>  如果对您有帮助，您可以点右上角 "Star" 支持一下 谢谢！ ^_^

>  或者您可以 "follow" 一下，我会不断开源更多的有趣的项目

>  开发环境 macOS 10.12.3  Chrome 55

>  特别感谢辰妹子，在百忙之中抽出时间和我一起完成了这个项目，辛苦了🌹

>  推荐一个 react + redux 开源项目，对react感兴趣的朋友赶紧去看看。[地址在这里](https://github.com/bailicangdu/react-pxq)

>  另外一个 vue2 + vuex 的入门项目，比当前的项目简单很多，非常适合入门练习。[地址在这里](https://github.com/bailicangdu/vue2-happyfri)




## 效果演示

#### (演示demo是上个月版本，使用的是模拟数据，ios会出现卡顿的现象，且很多已修复的bug未更新，最新效果请clone代码并运行，我会尽快更新demo);

[demo地址](http://test.fe.ptdev.cn/elm/)（请用chrome手机模式预览）
   
### 移动端扫描下方二维码

![](https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/elm_ewm.png)





## 目标功能
- [x] 定位功能 -- 完成
- [x] 选择城市 -- 完成
- [x] 搜索地址 -- 完成
- [x] 展示所选地址附近商家列表 -- 完成
- [x] 搜索美食，餐馆 -- 完成
- [x] 根据距离、销量、评分、特色菜、配送方式等进行排序和筛选 -- 完成
- [x] 餐馆食品列表页 -- 完成
- [x] 购物车功能 -- 完成
- [x] 店铺评价页面 -- 完成
- [x] 单个食品详情页面 -- 完成
- [x] 商家详情页 -- 完成
- [x] 登陆、注册 -- 完成
- [x] 修改密码 -- 完成
- [x] 个人中心 -- 完成
- [x] 发送短信、语音验证 -- 完成
- [x] 下单功能 -- 完成 ✨✨🎉🎉
- [x] 订单列表 -- 完成
- [x] 订单详情 -- 完成
- [x] 下载App -- 完成
- [x] 添加、删除、修改收货地址
- [x] 帐户信息 -- 完成
- [x] 服务中心 -- 完成
- [x] 红包 -- 完成
- [x] 上传头像，修改用户名 -- 完成
- [ ] 付款(很难实现)



## 项目布局
```
|-- build                            // webpack配置文件
|-- config                           // 项目打包路径
|-- elm                           	 // 上线项目文件，放在服务器即可正常访问
|-- screenshots                      // 项目截图
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
|   |-- config                       // 基本配置
|       |-- env.js                   // 环境切换配置
|       |-- fetch.js                 // 获取数据
|       |-- mUtils.js                // 常用的js方法
|       |-- rem.js                   // px转换rem
|   |-- images                       // 公共图片
|   |-- pages 
|       |-- balance                  // 余额页
|		  	|--children
|				|--detail			 //	余额说明
|       |-- benefit                  // 红包页
|		  |--children
|			|--commend			     //	推荐有奖
|			|--coupon			     //	代金券说明 
|			|--exchange			     //	兑换红包 
|			|--hbDescription		 //	红包说明 
|			|--hbHistory			 //	历史红包                       
|       |-- city                     // 当前城市页
|		|-- food                 	 // 食品筛选排序页
|		|-- confirmOrder             // 确认订单页
|		  |--children
|			|--invoice			     //	选择发票页
|			|--remark			     //	订单备注页
|			|--payment			     //	付款页
|			|--userValidation		 //	用户验证页
|			|--chooseAddress         //	选择地址页
|		      |--children
|				|--addAddress        //	添加地址页
|				  |--children
|					|--searchAddress // 搜索地址页
|       |-- download                 // 下载App
|       |-- find                     // 发现页
|       |-- forget                   // 忘记密码，修改密码页
|       |-- home                     // 首页
|       |-- login                    // 登陆注册页
|       |-- msite                    // 商铺列表页
|       |-- order                    // 订单列表页
|			|--children
|				|--orderDetail		 // 订单详情页
|       |-- points                   // 积分页
|		  	|--children
|				|--detail			 //	积分说明
|       |-- profile                  // 个人中心
|			|--children
|				|--balance			 // 我的余额
|				|--benefit			 // 我的优惠
|				|--info				 // 帐户信息
|		          |--children
|				    |--address       // 添加地址
|		              |--children
|				        |--add       // 新增地址
|		                  |--children
|				            |--addDetail// 搜索地址
|				|--points			 // 我的积分
|				|--setusername	     // 重置用户名
|       |-- search                   // 搜索页
|		|--service			         // 服务中心
|			|--children
|				|--questionDetail    // 问题详情
|       |-- shop                     // 商铺筛选页
|			|-- children             
|			  	|-- foodDetail       // 商铺信息页     
|			  	|-- shopDetail       // 单个商铺信息页
|					|-- children             
|				  		|-- shopSafe // 商铺认证信息页     
|       |-- vipcard                  // 会员卡办理页
|			|-- children             
|			  	|-- invoiceRecord    // 购买记录     
|			  	|-- useCart          // 使用卡号购买
|			  	|-- vipDescription   // 会员说明
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




## 总结

1、因为并不是elm官方，而且因为要开代理，必须在pc端打开，所以预计最多只能做到下单这一步，下单成功后可以在手机客户端查看并付款。

2、目前下单功能已经实现✨✨🎉🎉，下单功能完全采用官网真实数据，可以控制官网发短信或者打电话到指定的手机号码，下单后可以在手机App中查看并且付款。

3、一般涉及到money的网页逻辑都比较复杂，尤其像饿了么这样一个开放的平台，商家和食品种类繁多，页面与页面之间交互复杂，在写到 购物车 和 下单 功能时众多的数据和逻辑一度让人很头疼，又没有设计和接口文档，只能一步步摸索。

4、vue因其轻量级的框架在中小型项目中表现亮眼，在大型单页面应用中因为vuex的存在，表现依然出色，在处理复杂交互逻辑的时候，vuex的存在是不可或缺的。所以说利用 vue + vuex 完全可以去做大型的单页面项目。

5、在项目中并没有使用太多的插件，所有功能尽可能自己实现，对插件依赖太多并不是一件好事。

6、项目写到现在，从 登陆注册到、首页、搜索、商家列表、购物车、下单、订单列表、个人中心 一个流程走完之后、不但对vue的理解更深一层，而且对以后掌控大型项目的时候也有非常多的帮助，做一个实际的项目才能对自己有很大的提升。

7、已完成的页面共 35 个，项目已经慢慢开始接近尾声。



## 最后定一个小目标

1、后续我会用node.js构建一个模拟外卖平台的后台系统，满足这个项目所需要的后台接口，不再用饿了么的数据，并进行同构，实现前后端分离。

2、利用 react-native 写出跨 Android 和 IOS 的原生APP版本。

。。。敬请期待




## 部分项目截图



### 城市列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/home.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/home.gif" width="365" height="619"/>


### 搜索地址页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/city.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/city.gif" width="365" height="619"/>


### 商铺列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/msite.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/msite.gif" width="365" height="619"/>


### 商铺筛选页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/food.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/food.gif" width="365" height="619"/>


### 搜索页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/search.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/search.gif" width="365" height="619"/>


### 餐馆食品列表与购物车

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shop_cart.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shop_cart.gif" width="365" height="619"/>


### 餐馆评论页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/rating.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/rating.gif" width="365" height="619"/>


### 食品详情页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/fooddetail.png" width="365" height="619"/>


### 餐馆信息页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shopdetail1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shopdetail2.png" width="365" height="619"/>


### 登陆页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/login1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/login2.png" width="365" height="619"/>

### 重置密码页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/reset.png" width="365" height="619"/>

### 个人中心

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/profile.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/profile2.png" width="365" height="619"/>



### 确认订单页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirmOrder.gif" width="365" height="619"/>

### 订单列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/order.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/order2.png" width="365" height="619"/>

