import java.math.BigInteger;

class rsaverify {
 public static void main(String[] s) {
  rsaverify rv = new rsaverify();
  //rv.verifyexp();
  rv.verifykeys();
 }
 public void verifyexp() {
  BigInteger pu = new BigInteger("b06377b7", 16), 
   pr = new BigInteger("0bad04820638257d", 16), 
   m = new BigInteger("9ae0fcd26f95470d", 16), 
   t = new BigInteger("13c1e135", 16), c, d;
  System.out.println("t = " + t.toString(16));
  c = t.modPow(pu, m);
  System.out.println("c = " + c.toString(16));
  d = c.modPow(pr, m);
  System.out.println("d = " + d.toString(16));
 }
 public void verifykeys() {
  BigInteger p1 = new BigInteger("ccaf9a33", 16),
   p2 = new BigInteger("c1b4d9bf", 16),
   pu = new BigInteger("b06377b7", 16),
   pr = new BigInteger("0bad04820638257d", 16),
   one = new BigInteger("1", 16), p1mo, p2mo, t, m, pupr, r;
  System.out.println("p1 = " + p1.toString(16) + " is " + p1.isProbablePrime(100) + " prime.");
  System.out.println("p2 = " + p2.toString(16) + " is " + p2.isProbablePrime(100) + " prime.");
  m = p1.multiply(p2);
  p1mo = p1.subtract(one); 
  p2mo = p2.subtract(one); 
  t = p1mo.multiply(p2mo);
  System.out.println("m = " + m.toString(16));
  System.out.println("p1mo = " + p1mo.toString(16));
  System.out.println("p2mo = " + p2mo.toString(16));
  System.out.println("t = " + t.toString(16));
  System.out.println("gcd(t, pu) = " + t.gcd(pu).toString(16));
  System.out.println("gcd(t, pr) = " + t.gcd(pr).toString(16));
  System.out.println("gcd(t, pr) = " + t.gcd(pr).toString(16));
  pupr = pu.multiply(pr);
  r = pupr.mod(t);
  System.out.println("mod(pu * pr, t) = " + (pu.multiply(pr)).mod(t).toString(16));
  System.out.println("pupr = " + pupr.toString(16));
  System.out.println("r = " + r.toString(16));
 }
}
