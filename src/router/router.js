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
const invoice = r => require.ensure([], () => r(require('../page/confirmOrder/children/invoice')), 'invoice')
const chooseAddress = r => require.ensure([], () => r(require('../page/confirmOrder/children/chooseAddress')), 'chooseAddress')
const addAddress = r => require.ensure([], () => r(require('../page/confirmOrder/children/children/addAddress')), 'addAddress')
const searchAddress = r => require.ensure([], () => r(require('../page/confirmOrder/children/children/children/searchAddress')), 'searchAddress')
const foodDetail = r => require.ensure([], () => r(require('../page/shop/children/foodDetail')), 'foodDetail')
const shopDetail = r => require.ensure([], () => r(require('../page/shop/children/shopDetail')), 'shopDetail')
const shopSafe = r => require.ensure([], () => r(require('../page/shop/children/children/shopSafe')), 'shopSafe')
const info = r => require.ensure([], () => r(require('../page/profile/children/info')), 'info')
const balance = r => require.ensure([], () => r(require('../page/profile/children/balance')), 'balance')
const benefit = r => require.ensure([], () => r(require('../page/profile/children/benefit')), 'benefit')
const points = r => require.ensure([], () => r(require('../page/profile/children/points')), 'points')
const service = r => require.ensure([], () => r(require('../page/profile/children/service')), 'service')



export default [{
    path: '/',
    component: App, //顶层路由，对应index.html
    children: [ //二级路由。对应App.vue
        //地址为空时跳转home页面 
        {
            path: '',
            redirect: '/home'
        },
        //首页城市列表页
        {
            path: '/home',
            component: home
        },
        //当前选择城市页
        {
            path: '/city/:cityid',
            component: city
        },
        //所有商铺列表页
        {
            path: '/msite',
            component: msite,
        },
        //特色商铺列表页
        {
            path: '/food',
            component: food
        },
        //搜索页 
        {
            path: '/search/:geohash',
            component: search
        },
        //商铺详情页 
        {
            path: '/shop',
            component: shop,
            children: [{
                path: 'foodDetail', //食品详情页
                component: foodDetail,
            }, {
                path: 'shopDetail', //商铺详情页
                component: shopDetail,
                children: [{
                    path: 'shopSafe', //商铺安全认证页
                    component: shopSafe,
                }, ]
            }]
        },
        //确认订单页 
        {
            path: '/confirmOrder',
            component: confirmOrder,
            children: [{
                path: 'remark', //订单备注
                component: remark,
            }, {
                path: 'invoice', //发票抬头
                component: invoice,
            }, {
                path: 'chooseAddress', //选择地址
                component: chooseAddress,
                children: [{
                    path: 'addAddress', //添加地址
                    component: addAddress,
                    children: [{
                        path: 'searchAddress', //搜索地址
                        component: searchAddress,
                    }]
                }, ]
            }, ]
        },
        //登陆注册页 
        {
            path: '/login',
            component: login
        },
        //个人信息页 
        {
            path: '/profile',
            component: profile,
            children: [{
                path: 'info', //个人信息详情页
                component: info,
            }, {
                path: 'balance', //余额
                component: balance,
            }, {
                path: 'benefit', //我的优惠页
                component: benefit,
            }, {
                path: 'points', //我的积分页
                component: points,
            }, {
                path: 'service', //服务中心
                component: service,
            }]
        },
        //修改密码页 
        {
            path: '/forget',
            component: forget
        },
        //订单列表页 
        {
            path: '/order',
            component: order
        },
        //vip卡页   
        {
            path: '/vipcard',
            component: vipcard
        },
    ]
}]