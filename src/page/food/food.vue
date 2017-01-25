<template>
    <div class="food_container">
    	<head-top :head-title="headTitle" goBack="true"></head-top>
    	<section class="sort_container">
    		<div class="sort_item" :class="{choose_type:sortBy == 'food'}" @click="chooseType('food')">
    			<span>{{foodTitle}}</span>
    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
	    			<polygon points="0,3 10,3 5,8"/>
	    		</svg>
    		</div>
    		<div class="sort_item" :class="{choose_type:sortBy == 'sort'}" @click="chooseType('sort')">
    			<span>排序</span>
    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
	    			<polygon points="0,3 10,3 5,8"/>
	    		</svg>
    		</div>
    		<div class="sort_item sort_by_activity" :class="{choose_type:sortBy == 'activity'}" @click="chooseType('activity')">
    			<span>筛选</span>
    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
	    			<polygon points="0,3 10,3 5,8"/>
	    		</svg>
    		</div>
    	</section>
    	<section class="shop_list_container">
	    	<shop-list :restaurantCategoryId="restaurant_category_id"></shop-list>
    	</section>
    </div>    
</template>

<script>
import {mapState, mapMutations} from 'vuex'
import headTop from '../../components/header/head'
import shopList from '../../components/common/shoplist'
import {msiteAdress} from '../../service/getData'

export default {
	data(){
        return {
        	geohash: '', // city页面传递过来的地址geohash
            headTitle: '', // msiet页面头部标题
            foodTitle: '',
            restaurant_category_id: '', // 食品类型id值
            sortBy: '', // 筛选的条件
        }
    },
    async beforeMount(){
		this.geohash = this.$route.query.geohash;
		this.headTitle = this.$route.query.title;
		this.foodTitle = this.headTitle
		this.restaurant_category_id = this.$route.query.restaurant_category_id;
		//刷新页面时，vuex状态丢失，经度纬度需要重新获取	
		if (!this.latitude) {
	    	//获取位置信息
	    	let res = await msiteAdress(this.geohash);
	    	// 记录当前经度纬度
	    	this.RECORD_ADDRESS(res)
		}
    },
    mounted(){
        
    },
    components: {
    	headTop,
    	shopList
    },
    computed: {
    	...mapState([
			'latitude','longitude'
		])
    },
    methods: {
    	...mapMutations([
    		'RECORD_ADDRESS'
    	]),
    	chooseType(type){
    		if (this.sortBy !== type) {
    			this.sortBy = type;
    			if (type == 'food') {
					this.foodTitle = '分类';
    			}else{
    				this.foodTitle = this.headTitle;
    			}
    		}else{
    			this.sortBy = '';
    			if (type == 'food') {
    				this.foodTitle = this.headTitle;
    			}
    		}
    	}
    },
    watch: {

    }
}

</script>

<style lang="scss" scoped>
    @import '../../style/mixin';
	.food_container{
		padding-top: 3.6rem;
	}
	.sort_container{
		background-color: #fff;
		border-bottom: 0.025rem solid #f1f1f1;
		position: fixed;
		top: 1.95rem;
		right: 0;
		width: 100%;
		display: flex;
		padding: 0.3rem;
		z-index: 13;
		.sort_item{
			@include sc(0.55rem, #444);
			@include wh(33.3%, 1rem);
			text-align: center;
			line-height: 1rem;
			border-right: 0.025rem solid $bc;
			.sort_icon{
				vertical-align: middle;
				transition: all .3s;
				fill:#666;
			}
			
		}
		.choose_type{
			span{
				color: $blue;
			}
			.sort_icon{
				transform: rotate(180deg);
				fill:$blue;
			}
		}
		.sort_by_activity{
			border-right: none;
		}
	}
</style>
