const btnDarkMode = document.getElementById('darkmode')
const corpo = document.querySelector('body')
const resumo = document.getElementById('resumo')
const linguagensTitulo = document.querySelector('#linguangensTitulo')
const topicos = document.querySelectorAll('[id*=linguagensTopicos]')
const topicosTitulo = document.querySelectorAll('[id*=topicosTitulo]')
const topicosTexto = document.querySelectorAll('[id*=topicosTexto]')
const projetos = document.querySelector('#projetos')
const projetosTitulo = document.querySelector("#projetosTitulo")
const linguagensContainer = document.getElementById("linguagensId")
const rodape = document.getElementById("rodapeId")
const imgWhatsapp = document.getElementById("imgWhatsapp")
const imgGmail = document.getElementById("imgGmail")
const imgInstagram = document.getElementById("imgInstagram")
const imgLinkedin = document.getElementById("imgLinkedin")
const IdNodeJS = document.getElementById("IdNodeJS")
var statuDarkMode = true

const ativaLightMode = () =>{
    corpo.style.background.url = ''
    corpo.classList.add('body-light')
    resumo.classList.add('resumo-light')
    linguagensTitulo.style.color = 'black'
    topicos.forEach((topico) => topico.classList.add('topicos-light'))
    topicosTitulo.forEach((titulo) => titulo.style.color = 'black')
    topicosTexto.forEach((texto) => texto.style.color = 'black')
    projetos.classList.add('projetos-light')
    linguagensContainer.classList.add("linguagens-container-light")
    rodape.classList.add("rodape-light")


    imgGmail.src = 'img/rodape/gmail_black.png'
    imgInstagram.src = 'img/rodape/instagram_black_slim.png'
    imgLinkedin.src = 'img/rodape/linkedin_black_slim.png'
    imgWhatsapp.src = 'img/rodape/whatsapp_black_slim.png'
    IdNodeJS.src = 'img/topicos/logo-node-js.png'
    return false
}

const ativaDarkMode = () =>{
    corpo.classList.remove('body-light')
    resumo.classList.remove('resumo-light')
    linguagensTitulo.classList.remove('.linguagens-titulo-light')
    topicos.forEach((topico) => topico.classList.remove('topicos-light'))
    topicosTitulo.forEach((titulo) => titulo.classList.remove('topicos-titulo-light'))
    topicosTexto.forEach((texto) => texto.style.color = 'white')
    linguagensTitulo.style.color = 'white'
    projetos.classList.remove('projetos-light')
    projetosTitulo.classList.remove('projetos-titulo-light')
    rodape.classList.remove("rodape-light")

    imgGmail.src = 'img/rodape/gmail.png'
    imgInstagram.src = 'img/rodape/instagram.png'
    imgLinkedin.src = 'img/rodape/linkedin.png'
    imgWhatsapp.src = 'img/rodape/whatsapp.png'
    IdNodeJS.src = 'img/topicos/nodejsl_branca.png'
    IdNodeJS.style.width = '80px'
    return true
}
const mudarTema = () =>{

    statuDarkMode = statuDarkMode ? ativaLightMode() : ativaDarkMode();
}