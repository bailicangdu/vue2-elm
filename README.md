# 前言

vue2的发布后自己也研究了一段时间，奈何公司的技术栈是以react为主，没有机会好好利用vue2去做一个完整的项目。虽然写了几个demo，但和写一个完整的项目还是有很大差别的。于是自己想着用空余的时间写一个项目，选择了饿了么也只是因为经常用，熟悉它的布局。之前的饿了么官网是用angular1写的，最近才发现原来这段时间改成了vue1，看来饿了么也入了vue的坑。


首先遇到的问题当然是跨域，我们启动本地服务器是获取不到官网数据的，这是跨域的。当然解决的方法很多，jsonp，nginx反向代理，webpack-dev-server的proxy。这里我用的是 http-proxy-middleware 其实它们的原理是一样的。


__注：此项目纯属个人瞎搞，正常下单请选择饿了么官方客户端。__


# 技术栈
vue2 + vue-rotuer2 + vuex2 + webpack + ES6/7 + fetch + sass + flex + svg + http-proxy-middleware反向代理 

# 目标功能
- [x] 定位功能 -- 完成
- [x] 选择城市 -- 完成
- [x] 搜索地址 -- 完成
- [x] 展示所选地址附近商家列表 -- 完成
- [x] 搜索美食，餐馆 -- 完成
- [ ] 根据距离、销量、评分、特色菜等进行排序和筛选
- [ ] 单个餐馆的详细信息展示，商品列表，优惠活动
- [ ] 餐馆及单个商品的评价列表展示、筛选
- [ ] 购物车功能
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
|			|-- mixin.js             // 组件混合(包括：指令-下拉加载更多。。。)
|			|-- shoplist.js          // msite和shop页面的餐馆列表公共组件
|       |-- footer                   // 底部公共组件
|       |-- header                 	 // 头部公共组件
|
|   |-- config                       // 基本配置
|       |-- env.js                   // 环境切换配置
|       |-- fetch.js                 // 获取数据
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
|       |-- shop                     // 商铺详情页
|       |-- vipcard                  // vip卡办理页
|
|   |-- plugins                      // 引用的插件
|
|   |-- router                       // 路由配置
|
|   |-- service                      // 数据交互统一调配
|		|-- template                 // 存放临时数据
|		|-- getData.js               // 获取数据的统一调配文件，方便管理接口
|
|   |-- store                        // vuex的状态管理
|       |-- modules                  // store模块
|       |-- action.js                // 配置根actions
|       |-- getters.js               // 配置根getters
|       |-- index.js                 // 引用vuex，创建store
|       |-- mutation-types.js        // 定义常量muations名
|       |-- mutations.js             // 配置根mutations
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

访问 http://localhost:8088/
```


## 线上版本
```
npm run build

生成的elm文件夹放在服务器即可正常访问
```


# 个人感悟

ps:每次做完一个完整的项目总会试着去总结问题和心得，有些知识点似乎懂了，但是写出来的时候发现好多地方仍然没有理解，写出来能够加深印象，万一哪天忘了某些知识也可以回来看看，希望自己可以坚持写下去。


