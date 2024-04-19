import java.util.Scanner; 

public class Main
{
    int count;
	
	public class Produto
    {
        private int codigo;
        private String nome;
        private int quantidade;
        private String tipo;
        private float valor;
        
        //Primeiro construtor
        public Produto(String name)
        {   
            count++;
            codigo = count;
            nome = name;
        }
        
        //Segundo construtor
        public Produto(String name, int y)
        {
            count++;
            codigo = count;
            nome = name;
            quantidade = y;
        }
        
        //Terceiro construtor
        public Produto(String name, int y, String t, float z)
        {
            count++;
            codigo = count;
            nome = name;
            quantidade = y;
            tipo = t;
            valor = z;
        }
        
        public void vender(int x)
        {
            if(quantidade >= x){
                quantidade -= x;
                System.out.println("Valor total da venda: " + x*valor);
            } else{
                System.out.println("Não há produtos suficientes no estoque.");
            }
        }
        
        public void comprar(int x)
        {
            quantidade += x;
            System.out.println(quantidade);
        }
        
        public void comprar(int x, float y)
        {
            quantidade += x;
            valor = y;
            System.out.println(quantidade+" "+y);
        }
        
        public void consultar()
        {
            System.out.println("Código: "+ codigo);
            System.out.println("Nome: "+ nome);
            System.out.println("Quantidade em estoque: "+ quantidade);
            System.out.println("Tipo: "+ tipo);
            System.out.println("Valor: "+ valor);
        }
        
        public void inserir(String name, int q, String tp, float v)
        {
            nome = name;
            quantidade = q;
            tipo = tp;
            valor = v;   
        }
        
        public boolean igual(Produto p)
        {
            if((tipo == p.tipo) && (nome.equals(p.nome))){
                System.out.println("É igual.");
                return true;
            }
            else{
                System.out.println("Não é igual.");
                return false;
            }
        }
    }
    
	public static void main(String[] args) {
	    Main main = new Main();
		
		//Objeto construído com o primeiro construtor
		Scanner Obj2 = new Scanner(System.in);
		System.out.println("Insira o nome do produto:");
		Main.Produto my2 = main.new Produto(Obj2.nextLine());
		
		//Objeto construído com o segundo construtor
		Scanner Obj3 = new Scanner(System.in);
		System.out.println("Insira o nome e a quantidade do produto:");
		Main.Produto my3 = main.new Produto(Obj3.nextLine(), Obj3.nextInt());
		
		//Objeto construído com o terceiro construtor
		Scanner Obj4 = new Scanner(System.in);
		System.out.println("Insira o nome, a quantidade, o tipo e o valor do produto:");
		Main.Produto my4 = main.new Produto(Obj4.nextLine(), Obj4.nextInt(), Obj4.nextLine(), Obj4.nextFloat());
		
		my2.consultar();
		my3.consultar();
		my4.consultar();
	
		my2.vender(1);
		my2.comprar(2);
		my2.igual(my3);
		
		my3.vender(1);
		my3.comprar(2);
		my3.igual(my4);
		
		my4.vender(1);
		my4.comprar(2);
		my4.igual(my2);
		
		my2.consultar();
		my3.consultar();
		my4.consultar();
	}
}

