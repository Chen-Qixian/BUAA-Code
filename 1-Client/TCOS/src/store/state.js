let response = {1:'haha'}

try {
  if (localStorage.getItem('response')) {
    response = JSON.parse(localStorage.getItem('response')) || { 1:'haha' }
  }
} catch (e) {}

export default {
  response
}
