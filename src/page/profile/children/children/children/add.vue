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
        			<div class="input-new">
        				<input type="text" placeholder="小区/写字楼/学校等" readonly="readonly" />
        			</div>
        			 <div class="input-new">
        				<input type="text" placeholder="请填写详细送餐地址" @input="inputThingthree"/>
        				<p v-if="verifytwo">请详细填写送餐地址</p>
        			</div>
        			 <div class="input-new">
        				<input type="text" placeholder="请填写能够联系到您的手机号" />
        				<p v-if="verifytwo">请填写您的姓名</p>
        			</div>
        			<div class="input-new">
        				<input type="text" placeholder="备用联系电话（选填）" />
        				<p v-if="verifytwo">请填写您的姓名</p>
        			</div>
        		</section>
        		<section class="addbutton">
        			<button :class="{butopacity:butpart}">新增地址</button>
        		</section>
        	</form>
        </section>
    </div>
</template>

<script>
    import headTop from '../../../../../components/header/head'
    import {getImgPath} from '../../../../../components/common/mixin'

    export default {
      data(){
            return{
    			verify:false,
    			verifytwo:false,
    			butpart:false,
    			titletext:'新增地址',
    			message:'',
            }
        },
        created(){

        },
        mixins: [getImgPath],
        components: {
            headTop,

        },
        props:[],
        methods: {
            inputThing(){
            	(!this.message) ? this.verify=true : this.verify=false
            },
            inputThingthree(){
            	
            }
        }
    }
</script>
  
<style lang="scss" scoped>
    @import '../../../../../style/mixin';
  
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
    		opacity:1;
    	}
    }
</style>
