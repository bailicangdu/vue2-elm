# 前言

vue2的发布后自己也研究了一段时间，奈何公司的技术栈是以react为主，没有机会好好利用vue2去做一个完整的项目。虽然写了几个demo，但和写一个完整的项目还是有很大差别的。于是自己想着用空余的时间写一个项目，选择了饿了么也只是因为经常用，熟悉它的布局。之前的饿了么官网是用angular写的，最近才发现原来这段时间改成了vue，看来饿了么也入了vue的坑。

既然要写一个完整的项目，就要认真对待，所以除了付款其他所有功能都尽可能的实现，包括登陆、注册、个人中心、搜索、购物车、下单等等，也包括所有我能注意到的细节也都一并做出来，所以这绝对算是一个比较大的项目。

项目不使用模拟数据，所有数据均使用从官网实时获取的真实数据，最大程度实现和官网一样的功能，所以首先遇到的问题是跨域，我们启动本地服务器是获取不到官网数据的，这是跨域的。当然解决的方法很多，jsonp，nginx反向代理，webpack-dev-server的proxy，这里我用的是 http-proxy-middleware 原理都是一样的。


__注：此项目纯属个人瞎搞，正常下单请选择饿了么官方客户端。__


# 技术栈
vue2 + vue-rotuer2 + vuex + webpack + ES6/7 + fetch + sass + flex + svg + http-proxy-middleware反向代理 

# 目标功能
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
- [ ] 帐户信息
- [ ] 上传头像，修改用户名
- [ ] 积分商城
- [ ] 服务中心
- [ ] 添加、删除、修改收货地址
- [ ] 付款(很难实现)



# 项目布局
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
|   |-- pages                        // 页面组件
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
|       |-- find                     // 发现页
|       |-- forget                   // 忘记密码，修改密码页
|       |-- home                     // 首页
|       |-- login                    // 登陆注册页
|       |-- msite                    // 商铺列表页
|       |-- order                    // 订单列表页
|			|--children
|				|--orderDetail		 // 订单详情页
|       |-- profile                  // 个人中心
|			|--children
|				|--balance			 // 我的余额
|				|--benefit			 // 我的优惠
|				|--info				 // 帐户信息
|				|--points			 // 我的积分
|				|--service			 // 服务中心
|       |-- search                   // 搜索页
|       |-- shop                     // 商铺筛选页
|			|-- children             
|			  	|-- foodDetail       // 商铺信息页     
|			  	|-- shopDetail       // 单个商铺信息页
|					|-- children             
|				  		|-- shopSafe // 商铺认证信息页     
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


# 总结

1、因为并不是elm官方，而且因为要开代理，必须在pc端打开，所以预计最多只能做到下单这一步，下单成功后可以在手机客户端查看并付款。

2、目前下单功能已经实现✨✨🎉🎉，下单功能完全采用官网真实数据，可以控制官网发短信或者打电话到指定的手机号码，下单后可以在手机App中查看并且付款。

3、一般涉及到money的网页逻辑都比较复杂，尤其像饿了么这样一个开放的平台，商家和食品种类繁多，页面与页面之间交互复杂，在写到 购物车 和 下单 功能时众多的数据和逻辑一度让人很头疼，又没有设计和接口文档，只能一步步摸索。

4、vue因其轻量级的框架在中小型项目中表现亮眼，在大型单页面应用中因为vuex的存在，表现依然出色，在处理复杂交互逻辑的时候，vuex的存在是不可或缺的。所以说利用 vue + vuex 完全可以去做大型的单页面项目。

5、在项目中并没有使用太多的插件，所有功能尽可能自己实现，对插件依赖太多并不是一件好事。

6、项目写到现在，从 登陆注册到、首页、搜索、商家列表、购物车、下单、订单列表、个人中心 一个流程走完之后、不但对vue的理解更深一层，而且对以后掌控大型项目的时候也有非常多的帮助，做一个实际的项目才能对自己有很大的提升。

7、项目已完成的页面共 22 个，最主要的购物下单功能已经实现，预计全部完成后总页面数在 35 个左右。


# 效果演示

#### (演示效果为模拟数据，只做展示用，真实效果请下载项目并运行，方可获取真实的官网数据);

[demo地址](http://test.fe.ptdev.cn/elm/)（请用chrome手机模式预览）
   
### 移动端扫描下方二维码

![](https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/elm_ewm.png)



# 项目截图

## 城市列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/home.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/home.gif" width="365" height="619"/>


## 搜索地址页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/city.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/city.gif" width="365" height="619"/>


## 商铺列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/msite.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/msite.gif" width="365" height="619"/>


## 商铺筛选页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/food.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/food.gif" width="365" height="619"/>


## 搜索页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/search.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/search.gif" width="365" height="619"/>


## 餐馆食品列表与购物车

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shop_cart.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shop_cart.gif" width="365" height="619"/>


## 餐馆评论页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/rating.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/rating.gif" width="365" height="619"/>


## 食品详情页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/fooddetail.png" width="365" height="619"/>


## 餐馆信息页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shopdetail1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/shopdetail2.png" width="365" height="619"/>


## 登陆页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/login1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/login2.png" width="365" height="619"/>

## 重置密码页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/reset.png" width="365" height="619"/>

## 个人中心

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/profile.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/profile2.png" width="365" height="619"/>



## 确认订单页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm1.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm2.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm3.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm4.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm5.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/confirm6.png" width="365" height="619"/>

## 订单列表页

<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/order.png" width="365" height="619"/>
<img src="https://github.com/bailicangdu/vue2-elm/blob/master/screenshots/order2.png" width="365" height="619"/>





#### 其他页面正在开发中。。。


# 最后

>  本项目主要用于熟悉如何用 vue2 构建一个中大型项目

>  vue在开发的过程中的体验很不错，上手快、运行效率高，饿了么从angular转向vue不是没有道理的，看来vue会越来越火

>  开发环境 macOS 10.12.3  Chrome 55

>  如有问题请直接在 Issues 中提，或者可以加我的QQ 1264889788

>  另外推荐一个 react + redux 开源项目，对react感兴趣的朋友赶紧去看看。[地址在这里](https://github.com/bailicangdu/react-pxq)



##### 如果觉得不错，请star一下吧 😊
