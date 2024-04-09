public class Main
{
	
	public class Produto
    {
        private int codigo;
        private String nome;
        private int quantidade;
        private String tipo;
        private float valor;
        
        public Produto(int x)
        {
            codigo = x;    
        }
        
        public Produto(int x, String name)
        {
            codigo = x;
            nome = name;
        }
        
        public Produto(int x, String name, int y)
        {
            codigo = x;
            nome = name;
            quantidade = y;
        }
        
        public Produto(int x, String name, int y, String t, float z)
        {
            codigo = x;
            nome = name;
            quantidade = y;
            tipo = t;
            valor = z;
        }
        
        /*public void vender(int x)
        {
            if(quantidade >= x){
                
            } else{
                System.out.println("Não há produtos suficientes no estoque.");
            }
        }*/
    }
    
	public static void main(String[] args) {
	    Main main = new Main();
		Main.Produto my1 = main.new Produto(1);
		Main.Produto my2 = main.new Produto(2, "pera");
		Main.Produto my3 = main.new Produto(3, "banana", 5);
		Main.Produto my4 = main.new Produto(4, "caju", 7, "fruta", 5.50f);
		System.out.println(my1.codigo + " " + my1.nome + " " + my1.quantidade + " " + my1.tipo + " " + my1.valor);
		System.out.println(my2.codigo + " " + my2.nome + " " + my2.quantidade + " " + my2.tipo + " " + my2.valor);
		System.out.println(my3.codigo + " " + my3.nome + " " + my3.quantidade + " " + my3.tipo + " " + my3.valor);
		System.out.println(my4.codigo + " " + my4.nome + " " + my4.quantidade + " " + my4.tipo + " " + my4.valor);
	}
}

