//
// Below is an Actionscript excerpt I wrote which uses 
// Newton-Raphson iteration (from Calculus) to iteratively 
// "guess" an interest rate in the annuity equation given 
// the rest of the variables. I was pretty proud of it.
//
// Newton-Raphson Iteration
//   http://en.wikipedia.org/wiki/Newton's_method
//
// Annuity
//   http://en.wikipedia.org/wiki/Annuity_(finance_theory)
//

/**
 * Implements the Newton-Raphson method to iteratively 
 * determine the interest rate of an annuity.
 * 
 * @param nper <i>Number</i>. Number of periods.
 * @param pmt <i>Number</i>. Payment per period.
 * @param pv <i>Number</i>. Present value of annuity.
 * @param fv <i>Number</i>. Future value of annuity.
 * @param t <i>uint</i>. When interest is compounded; 
 * @param guess <i>Number</i>. The initial guess for 
 *   what the interest rate is. Defaults to 0.1. <i>Optional</i>.
 * 
 * @returns <i>Number</i>. The interest rate as determined. 0 if not found.
 * 
 * @author Zachary Jones <zacharytamas@gmail.com>
 */
public static function RATE(nper:Number, pmt:Number, pv:Number,
	fv:Number = 0, t:uint = 0, guess:Number = 0.1):Number {
	
	var found:Boolean = false;
	var valid:Boolean = true; // sometimes in calculation we return an invalid number, we need to stop if this happens
	var Try:Number; // the current Try (we're hoping to arrive on the right answer with this number)
	var newTry:Number;
	var maxTries:int = 100; // only try up to a hundred times
	var tryNum:int = 0; // number of try we're currently on
	
	var boundsLow:Number = 1 * Math.pow(10, -14); // if the precision is less than this number we assume we've found the rate
	var boundsHigh:Number = 1 * Math.pow(10, -7); // if the precision is less than this number we've most likely found it
	
	var mod:Number; var modPrime:Number;
	var term:Number; var termPrime:Number;
	
	// Normalization
	fv = fv - pmt*t;
	pv = pv + pmt*t;
	
	Try = (guess < -1.0) ? -1.0 : guess;
	
	while(valid && !found && tryNum < maxTries) {
		
		if (approximatelyEqual(Try,0)) {
			mod = nper;
			modPrime = nper * (nper-1)/2;
		} else {
			mod = (Math.pow(1+Try, nper)-1) / Try;
			modPrime = nper * Math.pow( 1+Try, nper-1 ) / Try - mod / Try;
		}
		
		term = fv + pv * Math.pow(1+Try, nper) + pmt * mod;
		termPrime = pv * nper*Math.pow(1+Try, nper-1) + pmt * modPrime;
		
		if (Math.abs(term) < boundsLow) {
			found = true;
		} else {
			
			newTry = (approximatelyEqual(termPrime, 0))
				? Try + 1.1 * boundsHigh
				: Try - term / termPrime;
			
			tryNum++;
			
			found = (Math.abs(newTry-Try) < boundsHigh);
			Try = newTry;
			valid = (Try >= -1.0);
		}
	}
	
	if (valid)
		return Try;
	else
		return 0.0;
}


/**
 * Determines the equality of two values with an accuracy of the magnitude of
 * the given values given scaled by 2^-48.
 * 
 * @param a Number 1.
 * @param b Number 2.
 * 
 * @returns <i>Boolean</i>.
 */
private static function approximatelyEqual(a:Number, b:Number):Boolean 
{
	if (a == b) // exact math
		return true;
	
	var x:Number = a-b;
	return (x < 0.0 ? -x : x)
	< ((a < 0.0 ? -a : a) * (1.0 / (Math.pow(16777216.0,2))));
}
