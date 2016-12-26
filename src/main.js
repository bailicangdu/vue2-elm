import Vue from 'vue'
import VueRouter from 'vue-router'
import routes from './router/router'
import store from './store/'
import ajax from './config/ajax'
import fetch from './config/fetch'
import {routerMode} from './config/env'
import './style/common.scss'
import './config/rem'
import {cityGuess} from './service/getData'

cityGuess().then(res => {
	console.log(res)
}).catch(err => {
	console.log(err)
})

// fetch('GET', '/shopping/restaurants',{
// 	latitude: 31.16407, 
// 	longitude: 121.38876, 
// 	offset: 0, 
// 	limit: 20, 
// 	'extras[]': 'activities',
// })


// ajax('get', '/v2/index_entry', {
// 	geohash: 'wtw3630xg5e', 
// 	group_type: 1, 
// 	'flags[]': 'F',
// })


// var obj = {
// 	mobile: '13681711254', 
// 	scene: 'login', 
// 	type: 'sms',
// }

// fetch('POST', '/v4/mobile/verify_code/send', obj)


// var loginObj = {
// 	code:567052,
// 	mobile: '13681711254',
// 	validate_token: '6a869f38c6d0a0bb5b023b35c5e35ceef8d710c1502369d56709b1632df830f8'
// }

// fetch('POST', '/v1/login/app_mobile', loginObj)


Vue.use(VueRouter)
const router = new VueRouter({
	mode: routerMode,
	routes
})

new Vue({
	router,
	store,
}).$mount('#app')