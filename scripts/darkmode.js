const btnDarkMode = document.getElementById('darkmode')
const body = document.querySelector('body')
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
const darkmode = document.getElementById("darkmode")
const github = document.getElementById("github")
const figma = document.getElementById("figma")
const listaDeitem = document.querySelectorAll("[id*=slides]")
const item = document.querySelector(".item")
const slideshow = document.querySelector("#conteinerSlides")

const imgs2 = [
    {'id' : '2', 'url': './img/meusprojetos/sessao_3_light.png'},
    {'id' : '1', 'url': './img/meusprojetos/sessao_1_light.png'},
    {'id' : '3', 'url': './img/meusprojetos/sessao_1_light.png'},
    {'id' : '4', 'url': './img/meusprojetos/sessao_2_light.png'},
]

var statuDarkMode = true
const ativaLightMode = () =>{
    body.style.background.url = ''
    body.classList.add('body-light')

    resumo.classList.add('resumo-light')
    linguagensTitulo.style.color = 'black'
    linguagensContainer.classList.add("linguagens-container-light")

    topicos.forEach((topico) => topico.classList.add('topicos-light'))
    topicosTexto.forEach((texto) => texto.style.color = 'black')
    topicosTitulo.forEach((titulo) => titulo.style.color = 'black')
    
    projetos.classList.add('projetos-light')
    rodape.classList.add("rodape-light")
    
    
    IdNodeJS.src = 'img/topicos/logo-node-js.png'
    imgGmail.src = 'img/rodape/gmail_black.png'
    imgWhatsapp.src = 'img/rodape/whatsapp_black_slim.png'
    imgLinkedin.src = 'img/rodape/linkedin_black_slim.png'
    imgInstagram.src = 'img/rodape/instagram_black_slim.png'
    
    github.classList.add('botoes-repositorio-light')
    figma.classList.add('botoes-repositorio-light')
    
    darkmode.innerHTML = 'Dark Mode'

    // item.classList.add("item-light")
    
    conteinerItens.innerHTML = ''
    console.log(document.querySelectorAll("[id*=slides"))
    
    loadImgs(imgs2, conteinerItens)
    slideshow.classList.add("slideshow-light")

    return false
}

const ativaDarkMode = () =>{
    body.classList.remove('body-light')
    resumo.classList.remove('resumo-light')
    
    linguagensTitulo.classList.remove('.linguagens-titulo-light')
    linguagensTitulo.style.color = 'white'
    linguagensContainer.classList.remove("linguagens-container-light")

    topicos.forEach((topico) => topico.classList.remove('topicos-light'))
    topicosTexto.forEach((texto) => texto.style.color = 'white')
    topicosTitulo.forEach((titulo) => titulo.style.color = 'white')

    projetos.classList.remove('projetos-light')
    projetosTitulo.classList.remove('projetos-titulo-light')

    rodape.classList.remove("rodape-light")

    IdNodeJS.src = 'img/topicos/nodejsl_branca.png'
    imgGmail.src = 'img/rodape/gmail.png'
    imgWhatsapp.src = 'img/rodape/whatsapp.png'
    imgLinkedin.src = 'img/rodape/linkedin.png'
    imgInstagram.src = 'img/rodape/instagram.png'
    IdNodeJS.style.width = '80px'

    github.classList.remove('botoes-repositorio-light')
    figma.classList.remove('botoes-repositorio-light')

    
    darkmode.innerHTML = 'Light Mode'

    conteinerItens.innerHTML = ''
    loadImgs(imgs, conteinerItens)

    return true
}



const mudarTema = () =>{

    statuDarkMode = statuDarkMode ? ativaLightMode() : ativaDarkMode();
}