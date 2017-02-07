import App from '../App'

const home = r => require.ensure([], () => r(require('../page/home/home')), 'home')
const city = r => require.ensure([], () => r(require('../page/city/city')), 'city')
const msite = r => require.ensure([], () => r(require('../page/msite/msite')), 'msite')
const search = r => require.ensure([], () => r(require('../page/search/search')), 'search')
const shop = r => require.ensure([], () => r(require('../page/shop/shop')), 'shop')
const login = r => require.ensure([], () => r(require('../page/login/login')), 'login')
const profile = r => require.ensure([], () => r(require('../page/profile/profile')), 'profile')
const forget = r => require.ensure([], () => r(require('../page/forget/forget')), 'forget')
const checkout = r => require.ensure([], () => r(require('../page/checkout/checkout')), 'checkout')
const order = r => require.ensure([], () => r(require('../page/order/order')), 'order')
const vipcard = r => require.ensure([], () => r(require('../page/vipcard/vipcard')), 'vipcard')
const food = r => require.ensure([], () => r(require('../page/food/food')), 'food')
const rating = r => require.ensure([], () => r(require('../page/shop/children/rating')), 'rating')


export default [{
    path: '/',   
    component: App,  //顶层路由，对应index.html
    children: [      //二级路由。对应App.vue
        { path: '', redirect: '/home' },               //地址为空时跳转home页面 
        { path: '/home', component: home },            //首页城市列表页
        { path: '/city/:cityid', component: city },    //当前选择城市页
        { path: '/msite', component: msite, },         //所有商铺列表页
        { path: '/food', component: food },            //特色商铺列表页
        { path: '/search/:geohash', component: search },   //搜索页
        { path: '/shop', component: shop ,
            children: [
                {
                    path: 'rating',
                    component: rating,
                }
            ]
        },            //商铺详情页
        { path: '/login', component: login },          //登陆注册页
        { path: '/profile', component: profile },      //个人信息页
        { path: '/forget', component: forget },        //修改密码页
        { path: '/checkout', component: checkout },    //确认订单页
        { path: '/order', component: order },          //订单列表页
        { path: '/vipcard', component: vipcard },      //vip卡页  
    ]
}]