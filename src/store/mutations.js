import {
	RECORD_ADDRESS,
	ADD_CART,
	REDUCE_CART,
	INIT_BUYCART,
	CLEAR_CART,
	RECORD_SHOPDETAIL,
	RECORD_USERINFO,
	GET_USERINFO,
	CONFIRM_REMARK,
	CONFIRM_INVOICE,
	CHOOSE_SEARCH_ADDRESS,
	SAVE_GEOHASH,
	CHOOSE_ADDRESS,
} from './mutation-types.js'
import {
	setStore,
	getStore,
} from '../config/mUtils'

export default {
	// 记录当前经度纬度
	[RECORD_ADDRESS](state, {
		latitude,
		longitude
	}) {
		state.latitude = latitude;
		state.longitude = longitude;
	},

	[RECORD_SHOPDETAIL](state, detail) {
		state.shopDetail = detail;
	},
	// 加入购物车
	[ADD_CART](state, {
		shopid,
		category_id,
		item_id,
		food_id,
		name,
		price,
		specs,
		packing_fee,
		sku_id,
		stock
	}) {
		let cart = state.cartList;
		if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id] && cart[shopid][category_id][item_id][food_id]) {
			cart[shopid][category_id][item_id][food_id]['num']++;
		} else if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id]) {
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else if (cart[shopid] && cart[shopid][category_id]) {
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else if (cart[shopid]) {
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		} else {
			cart[shopid] = {};
			cart[shopid][category_id] = {};
			cart[shopid][category_id][item_id] = {};
			cart[shopid][category_id][item_id][food_id] = {};
			cart[shopid][category_id][item_id][food_id]['num'] = 1;
			cart[shopid][category_id][item_id][food_id]['id'] = food_id;
			cart[shopid][category_id][item_id][food_id]['name'] = name;
			cart[shopid][category_id][item_id][food_id]['price'] = price;
			cart[shopid][category_id][item_id][food_id]['specs'] = specs;
			cart[shopid][category_id][item_id][food_id]['packing_fee'] = packing_fee;
			cart[shopid][category_id][item_id][food_id]['sku_id'] = sku_id;
			cart[shopid][category_id][item_id][food_id]['stock'] = stock;
		}
		//返回一个新的对象，否则计算属性无法监听到数据的变化
		state.cartList = Object.assign({}, cart);
		//存入localStorage
		setStore('buyCart', state.cartList);
	},
	// 移出购物车
	[REDUCE_CART](state, {
		shopid,
		category_id,
		item_id,
		food_id,
		name,
		price,
		specs,
	}) {
		let cart = state.cartList;
		if (cart[shopid] && cart[shopid][category_id] && cart[shopid][category_id][item_id] && cart[shopid][category_id][item_id][food_id]) {
			if (cart[shopid][category_id][item_id][food_id]['num'] > 0) {
				cart[shopid][category_id][item_id][food_id]['num']--;
				//返回一个新的对象，否则计算属性无法监听到数据的变化
				state.cartList = Object.assign({}, cart);
				//存入localStorage
				setStore('buyCart', state.cartList);
			} else {
				//商品数量为0，则清空当前商品的信息
				cart[shopid][category_id][item_id][food_id] = null;
			}
		}
	},
	//网页初始化时从本地缓存获取购物车数据
	[INIT_BUYCART](state) {
		let initCart = getStore('buyCart');
		if (initCart) {
			state.cartList = JSON.parse(initCart);
		}
	},
	//清空当前商品的购物车信息
	[CLEAR_CART](state, shopid) {
		state.cartList[shopid] = null;
		state.cartList = Object.assign({}, state.cartList);
		setStore('buyCart', state.cartList);
	},
	// 记录用户信息
	[RECORD_USERINFO](state, info) {
		state.userInfo = info;
		let validity = 30;
		let now = new Date();
		now.setTime(now.getTime() + validity * 24 * 60 * 60 * 1000);

		document.cookie = "USERID=" + info.user_id + ";expires=" + now.toGMTString();
		document.cookie = "SID=huRyTRd9QLij7NkbpHJoj3PQrx1eRiO6bAiw" + ";expires=" + now.toGMTString();
	},
	//获取用户信息存入vuex
	[GET_USERINFO](state, info) {
		if (!info.message) {
			state.userInfo = info;
		} else {
			state.userInfo = null;
		}
	},
	//记录订单页面用户选择的备注, 传递给订单确认页面
	[CONFIRM_REMARK](state, {
		remarkText,
		inputText
	}) {
		state.remarkText = remarkText;
		state.inputText = inputText;
	},
	//是否开发票
	[CONFIRM_INVOICE](state, invoice) {
		state.invoice = invoice;
	},
	//选择搜索的地址
	[CHOOSE_SEARCH_ADDRESS](state, place) {
		state.searchAddress = place;
	},
	//保存geohash
	[SAVE_GEOHASH](state, geohash) {
		state.geohash = geohash;
	},
	//选择的地址
	[CHOOSE_ADDRESS](state, {
		address,
		index
	}) {
		state.choosedAddress = address;
		state.addressIndex = index;
	}
}