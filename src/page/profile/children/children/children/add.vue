 <template>
  <div class="rating_page">
        <head-top head-title="新增地址" go-back='true'></head-top>
        <section class="adddetail">
        	<form action="">
        		<section class="ui-padding-block">
        			<div class="input-new">
        				<input type="text" placeholder="请填写你的姓名" :class="{verifies:verify}" v-model="message" @input="inputThing">
        				<p v-if="verify">请填写您的姓名</p>
        			</div>
        			<router-link to='/profile/info/address/add/addDetail' class="add-detail">
	        			<div class="input-new">
	        				<input type="text" placeholder="小区/写字楼/学校等" readonly="readonly" v-model="addAddress" />
	        			</div>
        			</router-link>
        			 <div class="input-new">
        				<input type="text" placeholder="请填写详细送餐地址" :class="{verifies:verifythree}" @input="inputThingthree" v-model="mesthree"/>
        				<p v-if="verifythree">{{sendaddress}}</p>
        			</div>
        			<div class="input-new">
        				<input type="text" placeholder="请填写能够联系到您的手机号" :class="{verifies:verifyfour}" v-model="telenum" @input="inputThingfour" />
        				<p v-if="verifyfour">{{telephone}}</p>
        			</div>
        			<div class="input-new">
        				<input type="text" placeholder="备用联系电话（选填）" v-model="standbytelenum" @input="inputThingfive" />
        				<p v-if="verifyfive">{{standbytele}}</p>
        			</div>
        		</section>
        		<section class="addbutton">
        			<button :class="{butopacity:butpart}" @click="submitThing">新增地址</button>
        		</section>
        	</form>
        </section>
        <router-view></router-view>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {getImgPath} from 'src/components/common/mixin'
    import {mapState, mapMutations} from 'vuex'
    export default {
      data(){
            return{
    			verify:false,			//第一个输入框
    			verifytwo:false,		//第二个输入框
    			verifythree:false,		//第三个输入框
    			verifyfour:false,		//第四个输入框
    			verifyfive:false,		//第五个输入框
    			butpart:false,			//新增地址按钮的透明度
    			sendaddress:'',
    			message:'',
    			mesthree:'',
    			telenum:'',
    			telephone:'',
    			standbytele:'',
    			standbytelenum:'',
    			addSearch:false,
    			newAddress:{},			//增加数组的元素
            }
        },
        created(){

        },
        mixins: [getImgPath],
        components: {
            headTop,

        },
        computed:{
             ...mapState([
                'addAddress','removeAddress','newAddress',
            ]),
            
        },
        props:[],
        methods: {
        	...mapMutations([
            	'ADD_ADDRESS'
            ]),
            inputThing(){
            	(!this.message) ? this.verify=true : this.verify=false;
            	this.bindThing()
            },
            inputThingthree(){
            	this.verifythree=true;
            	if(this.mesthree.length == 0){
            		this.sendaddress='请详细填写送餐地址';
            		
            	}else if(this.mesthree.length > 0 && this.mesthree.length <= 2){
            		this.sendaddress='送餐地址太短了，不能辨识';
            	}else{
            		this.sendaddress='';
            		this.verifythree=false;
            	}
            	this.bindThing()	
            },
            inputThingfour(){
            	this.verifyfour=true;
            	if((/^[1][358][0-9]{9}$/).test(this.telenum)){
            		this.verifyfour=false;
            	}else if(this.telenum == ''){
            		this.telephone="手机号不能为空"
            	}else{
            		this.telephone="请输入正确的手机号"
            	}
            	this.bindThing()
            },
            inputThingfive(){
            	this.verifyfive=true;
            	if((/^[1][358][0-9]{9}$/).test(this.standbytelenum) || this.standbytelenum == ''){
            		this.verifyfive=false;
            	}else{
            		this.standbytele="请输入正确的手机号"
            	}
            	this.bindThing()
            },
            bindThing(){
            	if(this.message && this.mesthree && !this.verifyfour){
            		this.butpart=true;
            	}else{
            		this.butpart=false;
            	}
            },
            submitThing(){
            	if(this.butpart){
            		this.ADD_ADDRESS({
                        name: this.message,
                        address: this.mesthree,
                        address_detail: this.addAddress,
                        geohash: 'wtw37r7cxep4',
                        phone: this.telenum,
                        phone_bk: this.standbytelenum,
                        poi: this.addAddress,
                        poi_type: 0,
                    });
            		this.$router.go(-1);
            	}
            }
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  	.router-slid-enter-active, .router-slid-leave-active {
  	    transition: all .4s;
  	}
  	.router-slid-enter, .router-slid-leave-active {
  	    transform: translateX(100%);
  	}
    .rating_page{
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #f2f2f2;
        z-index: 202;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .adddetail{
    	margin-top:.4rem;
    }
    .ui-padding-block{
    	background:#fff;
    	padding-top:.4rem;
    	.add-detail{display:block;}
    	.input-new{
    		padding-bottom:.4rem;
    		input{
    			display:block;
    			width:15rem;
    			font-size:.6rem;
    			margin:0 auto;
    			padding:.3rem;
    			background:#f2f2f2;
    			border:1px solid #ddd;
    			@include borderRadius(3px);
    		}
    		.verifies{
				border-color:#ea3106;
    		}
    		p{
				@include sc(.4rem,#ea3106);
				padding-left:.5rem;
				margin-top:.2rem;
    		}
    	}
    	
    }
    .addbutton{
    	margin:.6rem auto;
    	width:15rem;
    	background:#4cd964;
    	@include borderRadius(3px);
    	button{
    		width:100%;
    		@include sc(.6rem,#fff);
    		line-height:1.6rem;
    		background:none;
    		font-weight:700;
    		opacity:.6;
    	}
    	.butopacity{
    		transition: all .4s;
    		opacity:1;
    	}
    }
</style>
