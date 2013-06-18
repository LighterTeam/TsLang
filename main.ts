//要实现的Demo
class CA {
public
    CA() {
        printf(Init CA);
    }
    ~CA() {
        printf(UnInit CA);
    }
public
    int add(int a, int b) {
        return a+b;
    }
    
    static int gAdd(int a, int b) {
        return a+b;
    }
}

class CB  public  CA{
    virtual int add(int a, int b){
        return a+b + 100000;
    }
}

singleton class CCpublic CB{
    int a = 15;
    int b = 20;
} 

void main(){
    int a = 50;
    float b = 50.13f;
    double c = 66.66;
    var str = Hello!;
    var str2 = World!
    var str3 = str + str2;

    printf(GetType(str));

    CA cb = new CB();
    int num = cb.add(5, 8);
    printf("" + num);
    
    num = CB.gAdd(10, 10);
    printf("" + num);

    CC cc = CC.singleton();
    num = CB.gAdd(cc.a, cc.b);
}