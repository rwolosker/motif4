/*=========================================================

=========================================================*/
char cpfValida(cchar *cpf){
  String
    pcpf=PCHAR(cpf);
  int
    i,
    digito1,
    digito2,
    total;

  pcpf=pcpf.trim();
  if(!pcpf.len())
    return 0;
  if(11!=pcpf.len())
    return 0;
  //DIGITO_1:
    total=0;
    for(i=0;i<9;i++){
      int
        n=-48+pcpf[i];

      total+=((i+1)*n);
    }
    total%=11;
    digito1=total==10?0:total;
    if(digito1!=-48+pcpf[9])
      return 0;
  //DIGITO_2:
    total=0;
    for(i=0;i<9;i++){
      int
        n=-48+pcpf[i];

      total+=((i+1-1)*n);
    }
    total+=digito1*9;
    total%=11;
    digito2=total==10?0:total;
    if(digito2!=-48+pcpf[10])
      return 0;

  return 1
    &&!pcpf.equals("00000000000")
    &&!pcpf.equals("11111111111")
    &&!pcpf.equals("22222222222")
    &&!pcpf.equals("33333333333")
    &&!pcpf.equals("44444444444")
    &&!pcpf.equals("55555555555")
    &&!pcpf.equals("66666666666")
    &&!pcpf.equals("77777777777")
    &&!pcpf.equals("88888888888")
    &&!pcpf.equals("99999999999");
}
/*=========================================================

=========================================================*/
char cnpjValida(cchar *cnpj){
  String
    pcnpj=PCHAR(cnpj);

  pcnpj=pcnpj.trim();
  if(pcnpj.len()!=14)
    return 0;
  else{
    String
      pd1="543298765432",
      pd2="6543298765432";
    int
      total,d1,d2;

    total=0;
    for(int i=0;i<12;i++)
      total+=(pcnpj[i]-'0')*(pd1[i]-'0');
    d1=(d1=total%11)<2?0:11-d1;

    total=0;
    for(int i=0;i<13;i++)
      total+=(pcnpj[i]-'0')*(pd2[i]-'0');
    d2=(d2=total%11)<2?0:11-d2;

    return 1
      &&d1==pcnpj[12]-'0'
      &&d2==pcnpj[13]-'0';
  }
}
