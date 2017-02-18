import App from '../App'

const home = r => require.ensure([], () => r(require('../page/home/home')), 'home')
const city = r => require.ensure([], () => r(require('../page/city/city')), 'city')
const msite = r => require.ensure([], () => r(require('../page/msite/msite')), 'msite')
const search = r => require.ensure([], () => r(require('../page/search/search')), 'search')
const shop = r => require.ensure([], () => r(require('../page/shop/shop')), 'shop')
const login = r => require.ensure([], () => r(require('../page/login/login')), 'login')
const profile = r => require.ensure([], () => r(require('../page/profile/profile')), 'profile')
const forget = r => require.ensure([], () => r(require('../page/forget/forget')), 'forget')
const order = r => require.ensure([], () => r(require('../page/order/order')), 'order')
const vipcard = r => require.ensure([], () => r(require('../page/vipcard/vipcard')), 'vipcard')
const food = r => require.ensure([], () => r(require('../page/food/food')), 'food')
const confirmOrder = r => require.ensure([], () => r(require('../page/confirmOrder/confirmOrder')), 'confirmOrder')
const remark = r => require.ensure([], () => r(require('../page/confirmOrder/children/remark')), 'remark')
const foodDetail = r => require.ensure([], () => r(require('../page/shop/children/foodDetail')), 'foodDetail')
const shopDetail = r => require.ensure([], () => r(require('../page/shop/children/shopDetail')), 'shopDetail')
const shopSafe = r => require.ensure([], () => r(require('../page/shop/children/shopSafe')), 'shopSafe')
const info = r => require.ensure([], () => r(require('../page/profile/children/info')), 'info')
const balance = r => require.ensure([], () => r(require('../page/profile/children/balance')), 'balance')
const benefit = r => require.ensure([], () => r(require('../page/profile/children/benefit')), 'benefit')
const points = r => require.ensure([], () => r(require('../page/profile/children/points')), 'points')
const service = r => require.ensure([], () => r(require('../page/profile/children/service')), 'service')



export default [{
    path: '/',
    component: App, //顶层路由，对应index.html
    children: [ //二级路由。对应App.vue
        {
            path: '',
            redirect: '/home'
        }, //地址为空时跳转home页面 
        {
            path: '/home',
            component: home
        }, //首页城市列表页
        {
            path: '/city/:cityid',
            component: city
        }, //当前选择城市页
        {
            path: '/msite',
            component: msite,
        }, //所有商铺列表页
        {
            path: '/food',
            component: food
        }, //特色商铺列表页
        {
            path: '/search/:geohash',
            component: search
        }, //搜索页
        {
            path: '/shop',
            component: shop,
            children: [{
                path: 'foodDetail',
                component: foodDetail,
            }, {
                path: 'shopDetail',
                component: shopDetail,
                children: [{
                    path: 'shopSafe',
                    component: shopSafe,
                }, ]
            }]
        }, //商铺详情页
        {
            path: '/confirmOrder',
            component: confirmOrder,
            children: [{
                path: 'remark',
                component: remark,
            }, ]
        }, //确认订单页
        {
            path: '/login',
            component: login
        }, //登陆注册页
        {
            path: '/profile',
            component: profile,
            children: [{
                path: 'info',
                component: info,
            }, {
                path: 'balance',
                component: balance,
            }, {
                path: 'benefit',
                component: benefit,
            }, {
                path: 'points',
                component: points,
            }, {
                path: 'service',
                component: service,
            }]
        }, //个人信息页
        {
            path: '/forget',
            component: forget
        }, //修改密码页
        {
            path: '/order',
            component: order
        }, //订单列表页
        {
            path: '/vipcard',
            component: vipcard
        }, //vip卡页  
    ]
}]