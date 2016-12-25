import App from '../App'
const home = r => require.ensure([], () => r(require('../page/home')), 'home')
const city = r => require.ensure([], () => r(require('../page/city')), 'city')
const msite = r => require.ensure([], () => r(require('../page/msite')), 'msite')


export default [{
    path: '/',
    component: App,
    children: [{ 
        path: '', redirect: '/home' // 地址为空时跳转home页面
    }, {
        path: '/home',
        component: home  //首页城市列表页面
    }, {
        path: '/city',
        component: city  //当前选择城市页面
    }, {
        path: '/msite',
        component: msite  //所有商铺列表页面
    }]
}]