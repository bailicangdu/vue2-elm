import { RECORD_ADDRESS, ADD_CART, REDUCE_CART, INIT_BUYCART} from './mutation-types.js'
import {setStore, getStore} from '../config/mUtils'

export default {
	// 记录当前经度纬度
	[RECORD_ADDRESS] (state, {latitude, longitude}) {
		state.latitude = latitude;
		state.longitude = longitude;
	},
	// 加入购物车
	[ADD_CART] (state, {shopid, category_id, item_id, food_id}) {
		let cart = state.cartList;
		if (cart[shopid]&&cart[shopid][category_id]&&cart[shopid][category_id][item_id]&&cart[shopid][category_id][item_id][food_id]) {
			cart[shopid][category_id][item_id][food_id] ++;
		}else if(cart[shopid]&&cart[shopid][category_id]&&cart[shopid][category_id][item_id]){
			cart[shopid][category_id][item_id][food_id] = 1;
		}else if(cart[shopid]&&cart[shopid][category_id]){
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = 1;
		}else if(cart[shopid]){
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = 1;
		}else{
			cart[shopid] = {};
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = 1;
		}
		state.cartList = Object.assign({}, cart);
		setStore('buyCart', state.cartList);
	},
	[REDUCE_CART] (state, {shopid, category_id, item_id, food_id}) {
		let cart = state.cartList;
		if (cart[shopid]&&cart[shopid][category_id]&&cart[shopid][category_id][item_id]&&cart[shopid][category_id][item_id][food_id]) {
			if (cart[shopid][category_id][item_id][food_id] > 0) {
				cart[shopid][category_id][item_id][food_id] --;
				state.cartList = Object.assign({}, cart);
				setStore('buyCart', state.cartList);
			}
		}
	},
	[INIT_BUYCART](state){
		let initCart = getStore('buyCart');
		if (initCart) {
			state.cartList = JSON.parse(initCart);
		}
	}
}