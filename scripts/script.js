const imgs = [
    {'id' : '2.1', 'url': './img/meusprojetos/img3.png'},
    {'id' : '1.1', 'url': './img/meusprojetos/img1.png'},
    {'id' : '3.1', 'url': './img/meusprojetos/img1.png'},
    {'id' : '4.1', 'url': './img/meusprojetos/img2.png'},
]

const conteinerItens = document.querySelector('#conteiner')

const  loadImgs = (imgs, conteiner)=>{
    imgs.forEach(img => {
            let div = document.createElement('div');
            let imagem = document.createElement('img');
            let paragraf = document.createElement('p')

            paragraf.innerHTML = 'Confira Algumas Imagens do Projeto'
            imagem.src = `${img.url}`
            imagem.id = `slides${img.id}` 

            div.classList.add("item")
            div.id = `divSlides${img.id}`

            conteiner.appendChild(div)
            if(imagem.id == `slides1`){
                div.appendChild(paragraf)
            }
            div.appendChild(imagem)
            
    });
}

loadImgs( imgs, conteinerItens);

let listaDeItens = document.querySelectorAll('.item')

const nextSlide = () =>{
    conteinerItens.appendChild( listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

const previousSlide = () =>{
    const lastItem = listaDeItens[listaDeItens.length -1]
    conteinerItens.insertBefore( lastItem, listaDeItens[0])
    listaDeItens = document.querySelectorAll('.item')
}

document.querySelector('#previous').addEventListener('click', previousSlide)
document.querySelector('#next').addEventListener('click', nextSlide)