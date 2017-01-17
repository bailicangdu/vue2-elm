import {getStyle} from '../../config/mUtils'

export const loadMore = {
	directives:{
		'load-more':{
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
			        paddingBottom = getStyle(el,'paddingBottom');
			        marginBottom = getStyle(el,'marginBottom');
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
		}
	}
}