import App from '../App'

const home = r => require.ensure([], () => r(require('../page/home')), 'home')
const city = r => require.ensure([], () => r(require('../page/city')), 'city')
const msite = r => require.ensure([], () => r(require('../page/msite')), 'msite')
const search = r => require.ensure([], () => r(require('../page/search')), 'search')
const shop = r => require.ensure([], () => r(require('../page/shop')), 'shop')
const login = r => require.ensure([], () => r(require('../page/login')), 'login')
const profile = r => require.ensure([], () => r(require('../page/profile')), 'profile')
const forget = r => require.ensure([], () => r(require('../page/forget')), 'forget')
const checkout = r => require.ensure([], () => r(require('../page/checkout')), 'checkout')
const order = r => require.ensure([], () => r(require('../page/order')), 'order')
const vipcard = r => require.ensure([], () => r(require('../page/vipcard')), 'vipcard')
const food = r => require.ensure([], () => r(require('../page/msite/food')), 'food')


export default [{
    path: '/',   
    component: App,  //顶层路由，对应index.html
    children: [      //二级路由。对应App.vue
        { path: '', redirect: '/home' },               //地址为空时跳转home页面 
        { path: '/home', component: home },            //首页城市列表页
        { path: '/city/:ciytid', component: city },            //当前选择城市页
        { path: '/msite', component: msite,            //所有商铺列表页
            children:[ 
                { path: 'food', component: food },     //特色商铺列表页
                { path: '*', component: food }         //特色商铺列表页
            ]
        }, 
        { path: '/search', component: search },        //搜索页
        { path: '/shop', component: shop },            //商铺详情页
        { path: '/login', component: login },          //登陆注册页
        { path: '/profile', component: profile },      //个人信息页
        { path: '/forget', component: forget },        //修改密码页
        { path: '/checkout', component: checkout },    //确认订单页
        { path: '/order', component: order },          //订单列表页
        { path: '/vipcard', component: vipcard }       //vip卡页
    ]
}]