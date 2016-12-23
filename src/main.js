import Vue from 'vue'
import VueRouter from 'vue-router'
import routes from './router/router'
import store from './store/'
import ajax from './config/ajax'
import fetch from './config/fetch'
import './style/common'
import './config/rem'

// ajax('GET', '/shopping/restaurants',{
// 	latitude: 31.16407, 
// 	longitude: 121.38876, 
// 	offset: 0, 
// 	limit: 20, 
// 	'extras[]': 'activities',
// })


fetch('get', '/v2/index_entry', {
	geohash: 'wtw3630xg5e', 
	group_type: 1, 
	'flags[]': 'F',
}).then(res => {
	console.log(res)
}).catch(error => {
	console.log(error)
})


// var obj = {
// 	mobile: '13681711254', 
// 	scene: 'login', 
// 	type: 'sms',
// }

// var a = JSON.stringify(obj)
// ajax('POST', '/v4/mobile/verify_code/send', a)


// var loginObj = {
// 	code:212128,
// 	mobile: '13681711254',
// 	validate_token: '0af8df2bfd363e37cba0c616498e049777d7b118f13be41664491a05d8ad9da2'
// }
// var b = JSON.stringify(loginObj)

// ajax('POST', '/v1/login/app_mobile', b)


Vue.use(VueRouter)
const router = new VueRouter({
	routes
})

new Vue({
	router,
	store,
}).$mount('#app')