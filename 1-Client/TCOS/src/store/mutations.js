export default {
   newRes (state, res) {
    state.response = res
    try {
      localStorage.setItem('response', JSON.stringify(res))
    } catch (e) {}
  }
}