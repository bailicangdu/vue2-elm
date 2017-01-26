<template>
    <div class="food_container">
    	<head-top :head-title="headTitle" goBack="true"></head-top>
    	<section class="sort_container">
    		<div class="sort_item" :class="{choose_type:sortBy == 'food'}" >
    			<div class="sort_item_container" @click="chooseType('food')">
    				<div class="sort_item_border">
    					<span :class="{category_title: sortBy == 'food'}">{{foodTitle}}</span>
		    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
			    			<polygon points="0,3 10,3 5,8"/>
			    		</svg>
    				</div>
    			</div>
	    		<transition name="showlist" v-if="category">
	    			<section v-if="sortBy == 'food'" class="category_container sort_detail_type">
	    				<section class="category_left">
	    					<ul>
	    						<li v-for="(item, index) in category" :key="index" class="category_left_li" :class="{category_active:restaurant_category_id == item.id}" @click="selectCategoryName(item.id, index)">
									<section>
										<img :src="imgBaseUrl + subImgUrl(item.image_url)" v-if="index" class="category_icon">
										<span>{{item.name}}</span>
									</section>
									<section>
	    								<span class="category_count">{{item.count}}</span>
	    								<svg v-if="index" width="8" height="8" xmlns="http://www.w3.org/2000/svg" version="1.1" class="category_arrow" >
							    			<path d="M0 0 L6 4 L0 8"  stroke="#666" stroke-width="1.5" fill="none"/>
							    		</svg>
									</section>
	    						</li>
	    					</ul>
	    				</section>
	    				<section class="category_right">
	    					<ul>
	    						<li v-for="(item, index) in categoryDetail" :key="item.id" class="category_right_li" @click="getCategoryIds(item.id, item.name)" :class="{category_right_choosed: restaurant_category_ids == item.id || (!restaurant_category_ids)&&index == 0}">
	    							<span>{{item.name}}</span>
	    							<span>{{item.count}}</span>
	    						</li>
	    					</ul>
	    				</section>
	    			</section>
	    		</transition>	
    		</div>
    		<div class="sort_item" :class="{choose_type:sortBy == 'sort'}">
    			<div class="sort_item_container" @click="chooseType('sort')">
    				<div class="sort_item_border">
		    			<span :class="{category_title: sortBy == 'sort'}">排序</span>
		    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
			    			<polygon points="0,3 10,3 5,8"/>
			    		</svg>
    				</div>
    			</div>
	    		<transition name="showlist">
	    			<section v-if="sortBy == 'sort'" class="sort_detail_type">
	    				<ul class="sort_list_container" @click="sortList($event)">
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#default"></use>
								</svg>
	    						<p data="0" :class="{sort_select: sortByType == 0}">
	    							<span>智能排序</span>
	    							<svg v-if="sortByType == 0">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
	    						</p>
	    					</li>
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#distance"></use>
								</svg>
	    						<p data="5" :class="{sort_select: sortByType == 5}">
	    							<span>距离最近</span>
	    							<svg v-if="sortByType == 5">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
	    						</p>
	    					</li>
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#hot"></use>
								</svg>
	    						<p data="6" :class="{sort_select: sortByType == 6}">
	    							<span>销量最高</span>
	    							<svg v-if="sortByType == 6">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
	    						</p>
	    					</li>
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#price"></use>
								</svg>
	    						<p data="1" :class="{sort_select: sortByType == 1}">
	    							<span>起送价最低</span>
	    							<svg v-if="sortByType == 1">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
								</p>
	    					</li>
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#speed"></use>
								</svg>
	    						<p data="2" :class="{sort_select: sortByType == 2}">
	    							<span>配送速度最快</span>
	    							<svg v-if="sortByType == 2">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
	    						</p>
	    					</li>
	    					<li class="sort_list_li">
	    						<svg>
									<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#rating"></use>
								</svg>
	    						<p data="3" :class="{sort_select: sortByType == 3}">
	    							<span>评分最高</span>
	    							<svg v-if="sortByType == 3">
										<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#selected"></use>
									</svg>
	    						</p>
	    					</li>
	    				</ul>
	    			</section>
	    		</transition>	
    		</div>
    		<div class="sort_item" :class="{choose_type:sortBy == 'activity'}">
    			<div class="sort_item_container" @click="chooseType('activity')">
	    			<span :class="{category_title: sortBy == 'activity'}">筛选</span>
	    			<svg width="10" height="10" xmlns="http://www.w3.org/2000/svg" version="1.1" class="sort_icon">
		    			<polygon points="0,3 10,3 5,8"/>
		    		</svg>
    			</div>
	    		<transition name="showlist">
	    			<section v-if="sortBy == 'activity'" class="sort_detail_type">
	    				efsfsdf
	    			</section>
	    		</transition>
    		</div>
    	</section>
    	<transition name="showcover">
    		<div class="back_cover" v-show="sortBy"></div>
    	</transition>
    	<section class="shop_list_container">
	    	<shop-list :restaurantCategoryId="restaurant_category_id" :restaurantCategoryIds="restaurant_category_ids" :sortByType='sortByType' v-if="latitude"></shop-list>
    	</section>
    </div>    
</template>

<script>
import {mapState, mapMutations} from 'vuex'
import headTop from '../../components/header/head'
import shopList from '../../components/common/shoplist'
import {imgBaseUrl} from '../../config/env'
import {msiteAdress, foodCategory} from '../../service/getData'

export default {
	data(){
        return {
        	geohash: '', // city页面传递过来的地址geohash
            headTitle: '', // msiet页面头部标题
            foodTitle: '', // 排序左侧头部标题
            imgBaseUrl, //图片域名地址
            restaurant_category_id: '', // 食品类型id值
            restaurant_category_ids: '', //筛选类型的id
            sortBy: '', // 筛选的条件
            category: null, // category分类左侧数据
            categoryDetail: null, // category分类右侧的详细数据
            sortByType: null, // 根据何种方式排序
        }
    },
    async beforeMount(){
		this.geohash = this.$route.query.geohash;
		this.headTitle = this.$route.query.title;
		this.foodTitle = this.headTitle
		this.restaurant_category_id = this.$route.query.restaurant_category_id;
		//防止刷新页面时，vuex状态丢失，经度纬度需要重新获取，并存入vuex	
		if (!this.latitude) {
	    	//获取位置信息
	    	let res = await msiteAdress(this.geohash);
	    	// 记录当前经度纬度
	    	this.RECORD_ADDRESS(res);
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
    	async chooseType(type){
    		if (this.sortBy !== type) {
    			this.sortBy = type;
    			if (type == 'food') {
					this.foodTitle = '分类';
					if (!this.category) {
						this.category = await foodCategory(this.latitude, this.longitude);
						this.category.forEach(item => {
							if (this.restaurant_category_id == item.id) {
								this.categoryDetail = item.sub_categories;
							}
						})
					}
    			}else{
    				this.foodTitle = this.headTitle;
    			}
    		}else{
    			this.sortBy = '';
    			if (type == 'food') {
    				this.foodTitle = this.headTitle;
    			}
    		}
    	},
    	//传递过来的图片地址需要处理后才能正常使用
		subImgUrl(path){
			let suffix;
			if (path.indexOf('jpeg') !== -1) {
				suffix = '.jpeg'
			}else{
				suffix = '.png'
			}
			let url = '/' + path.substr(0,1) + '/' + path.substr(1,2) + '/' + path.substr(3) + suffix;
			return url
		},
		selectCategoryName(id, index){
			if (index === 0) {
				this.restaurant_category_ids = null;
				this.sortBy = '';
			}else{
				this.restaurant_category_id = id;
				this.categoryDetail = this.category[index].sub_categories;

			}
		},
		getCategoryIds(id, name){
			this.restaurant_category_ids = id;
			this.sortBy = '';
			this.foodTitle = this.headTitle = name;

		},
		sortList(event){
			this.sortByType = event.target.getAttribute('data');
			this.sortBy = '';
		},
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
		z-index: 13;
		box-sizing: border-box;
		.sort_item{
			@include sc(0.55rem, #444);
			@include wh(33.3%, 1.6rem);
			text-align: center;
			line-height: 1rem;
			.sort_item_container{
				@include wh(100%, 100%);
				position: relative;
				z-index: 14;
				background-color: #fff;
				box-sizing: border-box;
				padding-top: .3rem;
				.sort_item_border{
					height: 1rem;
					border-right: 0.025rem solid $bc;
				}
			}
			.sort_icon{
				vertical-align: middle;
				transition: all .3s;
				fill:#666;
			}
			
		}
		.choose_type{
			.sort_item_container{

				.category_title{
					color: $blue;
				}
				.sort_icon{
					transform: rotate(180deg);
					fill:$blue;
				}
			}
		}
		.showlist-enter-active, .showlist-leave-active {
			transition: all .3s;
			transform: translateY(0);
		}
		.showlist-enter, .showlist-leave-active {
			opacity: 0;
			transform: translateY(-100%);
		}
		.sort_detail_type{
			width: 100%;
			position: absolute;
			display:flex;
			top: 1.6rem;
			left: 0;
			border-top: 0.025rem solid $bc;
			background-color: #fff;
		}
		.category_container{
			.category_left{
				flex: 1;
				background-color: #f1f1f1;
				height: 16rem;
				overflow-y: auto;
				span{
					@include sc(0.5rem, #666);
					line-height: 1.8rem;
				}
				.category_left_li{
					@include fj;
					padding:0 0.5rem;
					.category_icon{
						@include wh(.8rem, .8rem);
						vertical-align: middle;
						margin-right: .2rem;
					}
					.category_count{
						background-color: #ccc;
						@include sc(.4rem, #fff);
						padding: .08rem .1rem 0;
						border: 0.025rem solid #ccc;
						border-radius: 0.3rem;
						vertical-align: middle;
						margin-right: 0.25rem;
					}
					.category_arrow{
						vertical-align: middle;
					}
				}
				.category_active{
					background-color: #fff;
				}
			}
			.category_right{
				flex: 1;
				background-color: #fff;
				padding-left: 0.5rem;
				height: 16rem;
				overflow-y: auto;
				.category_right_li{
					@include fj;
					height: 1.8rem;
					line-height: 1.8rem;
					padding-right: 0.5rem;
					border-bottom: 0.025rem solid $bc;
					span{
						color: #666;
					}
				}
				.category_right_choosed{
					span{
						color: $blue;
					}
				}
			}
		}
		.sort_list_container{
			width: 100%;
			.sort_list_li{
				height: 2.5rem;
				display: flex;
				align-items: center;
				svg{
					@include wh(0.7rem, 0.7rem);
					margin:0 .3rem 0 .8rem;
				}
				p{
					line-height: 2.5rem;
					flex: auto;
					text-align: left;
					text-indent: 0.25rem;
					border-bottom: 0.025rem solid $bc;
					@include fj;
					align-items: center;
					span{
						color: #666;
					}
				}
				.sort_select{
					span{
						color: $blue;
					}
				}
			}
		}
	}
	.showcover-enter-active, .showcover-leave-active {
		transition: opacity .3s
	}
	.showcover-enter, .showcover-leave-active {
		opacity: 0
	}
	.back_cover{
		position: fixed;
		@include wh(100%, 100%);
		z-index: 10;
		background-color: rgba(0,0,0,0.3);
	}
</style>
