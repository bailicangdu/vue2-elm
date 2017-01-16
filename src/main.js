import Vue from 'vue'
import VueRouter from 'vue-router'
import routes from './router/router'
import store from './store/'
import {routerMode} from './config/env'
import './style/common.scss'
import './config/rem'
import * as Tool from './config/mUtils'

Vue.use(VueRouter)
const router = new VueRouter({
	routes,
	mode: routerMode,
	strict: process.env.NODE_ENV !== 'production'
})

Vue.directive('load-more',{
	bind: (el, binding) => {
		let windowHeight = window.screen.height;
		let height;
		let setTop;
		let paddingBottom;
		let marginBottom;
	    let requestFram;
	    let oldScrollTop;

		el.addEventListener('touchstart',() => {
	        height = el.offsetHeight;
	        setTop = el.offsetTop;
	        paddingBottom = parseInt(Tool.getStyle(el,'paddingBottom'));
	        marginBottom = parseInt(Tool.getStyle(el,'marginBottom'));
	    },false)

	    el.addEventListener('touchmove',() => {
	       loadMore();
	    },false)

	    el.addEventListener('touchend',() => {
	       	oldScrollTop = document.body.scrollTop;
	        moveEnd()
	    },false)
	    
	    const moveEnd = () => {
	        requestFram = requestAnimationFrame(() => {
	            if (document.body.scrollTop != oldScrollTop) {
	                oldScrollTop = document.body.scrollTop;
	                moveEnd()
	            }else{
	            	cancelAnimationFrame(requestFram);
	            	height = el.offsetHeight;
	                loadMore();
	            }
	        })
	    }

	    const loadMore = () => {
	        if (document.body.scrollTop + windowHeight >= height + setTop + paddingBottom + marginBottom) {
	            binding.value();
	        }
	    }
	}
})

new Vue({
	router,
	store,
}).$mount('#app')