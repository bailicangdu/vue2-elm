<template>
	<div>
		<a id="top"></a>
		<ul v-load-more="loaderMore"  >
			<router-link :to="{path: 'food', query:{}}" v-for="item in shopListArr" tag='li' :key="item.id" class="shop_li">
				<section>
					<img :src="imgBaseUrl + subImgUrl(item.image_path)" class="shop_img">
				</section>
				<hgroup class="shop_right">
					<header class="shop_detail_header">
						<h4 :class="item.is_premium? 'premium': ''" class="" class="shop_title ellipsis">{{item.name}}</h4>
						<ul class="shop_detail_ul">
							<li v-for="item in item.supports" key="item.id" class="supports">{{item.icon_name}}</li>
						</ul>
					</header>
					<h5 class="rating_order_num">
						<section class="rating_order_num_left">
							<section class="rating_section">
								<div class="rating_container">
									<span class="star_container">
										<svg class="grey_fill" v-for="num in 5" key="num">
											<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#star"></use>
										</svg>
									</span>
									<div :style="'width:' + item.rating*2/5 + 'rem'" class="star_overflow">
										<span class="star_container" >
											<svg  class="orange_fill" v-for="num in 5" key="num">
												<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#star"></use>
											</svg>
										</span>
									</div>
								</div>
								<span class="rating_num">{{item.rating}}</span>
							</section>
							<section class="order_section">
								月售{{item.recent_order_num}}单
							</section>
						</section>
						<section class="rating_order_num_right" v-if="item.delivery_mode">
							<span class="delivery_style delivery_left">{{item.delivery_mode.text}}</span>
							<span class="delivery_style delivery_right">准时达</span>
						</section>
					</h5>
					<h5 class="fee_distance">
						<section class="fee">
							¥{{item.float_minimum_order_amount}}起送 
							<span class="segmentation">/</span>
							{{item.piecewise_agent_fee.tips}}
						</section>
						<section class="distance_time">
							<span>{{item.distance > 1000? (item.distance/1000).toFixed(2) + 'km': item.distance + 'm'}}
								<span class="segmentation">/</span>
							</span>
							<span class="order_time">{{item.order_lead_time}}分钟</span>
						</section>
					</h5>
				</hgroup>
			</router-link>
		</ul>
		<a class="return_top" @click="backTop" v-if="showBack">
			<svg class="back_top_svg">
				<use xmlns:xlink="http://www.w3.org/1999/xlink" xlink:href="#backtop"></use>
			</svg>
		</a>
		<footer class="loader_more" v-show="preventRepeatReuqest">正在加载更多商家...</footer>
	</div>
</template>

<script>

import {mapState} from 'vuex'
import {imgBaseUrl} from '../../config/env'
import {msiteShopList} from '../../service/getData'
import * as Tool from '../../config/mUtils'

export default {
	data(){
		return {
			offset: 0, // 批次加载店铺列表，每次加载20个 limit = 20
			shopListArr:[], // 店铺列表数据
			imgBaseUrl, //图片域名地址
			preventRepeatReuqest: false, //到达底部加载数据，防止重复加载
			showBack: false, //显示返回顶部按钮
		}
	},
	async mounted(){
		//获取数据
		this.shopListArr = await msiteShopList(this.latitude, this.longitude, this.offset);
		Tool.showBack(status => {
			this.showBack = status;
		})
	},
	props: [],
	components: {

	},
	computed: {
		...mapState([
			'latitude','longitude'
		])
	},
	methods: {
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
		//到达底部加载更多数据
		async loaderMore(){
			//防止重复请求
			if (this.preventRepeatReuqest) {
				return 
			}
			this.preventRepeatReuqest = true;
			this.offset += 20;
			let res = await msiteShopList(this.latitude, this.longitude, this.offset);
			this.shopListArr = this.shopListArr.concat(res);

			//当获取数据小于20，说明没有更多数据，不需要再次请求数据
			if (res.length < 20) {
				return
			}
			this.preventRepeatReuqest = false;
		},
		//返回顶部
		backTop(){
			window.scrollTo(0,0);
		}
	}
}
</script>

<style lang="scss" scoped>
	@import '../../style/mixin.scss';
	.shop_li{
		display: flex;
		border-bottom: 0.025rem solid #f1f1f1;
		padding: 0.7rem 0.4rem;
	}
	.shop_img{
		@include wh(2.7rem, 2.7rem);
		display: block;
		margin-right: 0.4rem;
	}
	.shop_right{
		flex: auto;
		.shop_detail_header{
			@include fj;
			align-items: center;
			.shop_title{
				width: 8.5rem;
				color: #333;
				@include font(0.65rem, 0.65rem, 'PingFangSC-Regular');
				font-weight: 700;
			}
			.premium::before{
				content: '品牌';
				display: inline-block;
				font-size: 0.5rem;
				background-color: #ffd930;
				padding: 0 0.1rem;
				border-radius: 0.1rem;
				margin-right: 0.2rem;
				vertical-align: top;
			}
			.shop_detail_ul{
				display: flex;
				.supports{
					@include sc(0.5rem, #999);
					border: 0.025rem solid #f1f1f1;
					padding: 0.04rem;
					border-radius: 0.08rem;
					margin-left: 0.05rem;
				}
			}
		}
		.rating_order_num{
			@include fj(space-between);
			height: 0.6rem;
			margin-top: 0.52rem;
			.rating_order_num_left{
				@include fj(flex-start);
				.rating_section{
					display: flex;
					.rating_container{
						position: relative;
						width: 2rem;
						.star_overflow{
							overflow: hidden;
							position: relative;
							height: 100%;
						}
						.star_container{
							position: absolute;
							display: flex;
							width: 2rem;
							height: 0.4rem;
							top: -0.02rem;
							.grey_fill{
								fill: #d1d1d1;
							}
							.orange_fill{
								fill: #ff9a0d;
							}
						}
					}
					.rating_num{
						@include sc(0.4rem, #ff6000);
						margin: 0 0.2rem;
					}
				}
				.order_section{
						@include sc(0.4rem, #666);
				}
			}
			.rating_order_num_right{
				display: flex;
				.delivery_style{
					font-size: 0.4rem;
					padding: 0.04rem 0.08rem 0;
					border-radius: 0.08rem;
					margin-left: 0.08rem;
				}
				.delivery_left{
					color: #fff;
					background-color: $blue;
					border: 0.025rem solid $blue;
				}
				.delivery_right{
					color: $blue;
					border: 0.025rem solid $blue;
				}
			}
		}
		.fee_distance{
			margin-top: 0.52rem;
			@include fj;
			@include sc(0.5rem, #666);
			.distance_time{
				.order_time{
					color: $blue;
				}
			}
			.segmentation{
				color: #ccc;
			}
		}
	}
	.loader_more{
		@include font(0.6rem, 3);
		text-align: center;
	    color: #999;
	}
	.return_top{
		position: fixed;
		bottom: 3rem;
		right: 1rem;
		.back_top_svg{
			@include wh(2rem, 2rem);
		}
	}

</style>