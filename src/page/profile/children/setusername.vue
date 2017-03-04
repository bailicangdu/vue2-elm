 <template>
    <div class="rating_page">
        <head-top head-title="修改用户名" go-back='true'></head-top>
        <section class="setname">
            <section class="setname-top">
                <input type="text" placeholder="输入用户名" :class="{'setname-input':bordercolor}" @input="inputThing" v-model="inputValue">
                <div>
                    <p v-if="earn">用户名只能修改一次（5-24字符之间）</p>
                    <p class="unlikep" v-else>用户名长度在5到24位之间</p>
                </div>
            </section>
            <section class="reset">
                <button :class="{fontopacity:opacityall}" @click="resetName">确认修改</button>
            </section>
        </section>
    </div>
</template>

<script>
    import headTop from 'src/components/header/head'
    import {getImgPath} from 'src/components/common/mixin'
    import {mapMutations,mapState} from 'vuex'
    export default {
      data(){
            return{
                earn: true,     //输入框提醒
                bordercolor: false,  //输入框边框颜色
                opacityall: false,   //字体透明度
                inputValue: '',       //输入框的内容
                newusername: ''         //新用户名
            }
        },
        created(){

        },
        mixins: [getImgPath],
        components: {
            headTop,

        },
        mounted(){
            
        },
        computed:{
            ...mapState([
                'userInfo'
            ])
        },
        props:[],
        methods: {
            ...mapMutations([
                'RETSET_NAME'
            ]),
            inputThing(){
                if(this.inputValue.length <5 || this.inputValue.length>24){
                    this.earn=false;
                    this.bordercolor=true;
                    this.opacityall=false;
                }else{
                    this.earn=true;
                    this.bordercolor=false;
                    this.opacityall=true;
                }
            },
            resetName(){
                this.RETSET_NAME(this.inputValue);
                this.$router.go(-1);
            }
        }
    }
</script>
  
<style lang="scss" scoped>
    @import 'src/style/mixin';
  
    .rating_page{
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        z-index: 202;
        background:#f2f2f2;
        padding-top: 1.95rem;
        p, span{
            font-family: Helvetica Neue,Tahoma,Arial;
        }
    }
    .setname{
        width:15.2rem;
        margin:0 auto;
        .setname-top{
            padding-top:.4rem;
            input{
                background:none;
                width:15.2rem;
                border:1px solid #ddd;
                @include borderRadius(2px);
                padding:.2rem .1rem;
                line-height:1.2rem;
                font-size:.7rem;
                display:block;
            }
            .setname-input{
                border-color:#ea3106;
            }
            p{
                width:100%;
                @include sc(.4rem,#666);
                padding:.4rem 0 1rem;
            }
            .unlikep{
                @include sc(.58rem,#ea3106);
                padding-top:.1rem;
            }
        }
        .reset{
            width:100%;
            background:#3199e8;
            button{
                display:block;
                width:100%;
                background:none;
                line-height:2rem;
                @include sc(.7rem,#fff);
                opacity:.6;
                transition: all 1s;
            }
            .fontopacity{
                transition: all 1s;
                opacity:1;
            }
        }
    }
</style>
