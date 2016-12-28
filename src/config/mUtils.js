/**
 * 存储localStorage
 */
const setStore = (name, content) => {
	if (!name) return;
	if (typeof content !== 'string') {
		content = JSON.stringify(content)
	}
	window.localStorage.setItem(name, content);
}

/**
 * 获取localStorage
 */
const getStore = name => {
	if (!name) return;
	return window.localStorage.getItem(name);
}

/**
 * 删除localStorage
 */
const removeStore = name => {
	if (!name) return;
	window.localStorage.removeItem(name);
}



export {setStore, getStore, removeStore}