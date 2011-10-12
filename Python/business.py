#
# This is a Django (http://www.djangoproject.com/) 
# database model describing a Business that I developed 
# for a client's local "marketplace" type site.
#

"""A business in the Directory."""

from geopy import geocoders
import datetime

from django.db import models
from django.core.urlresolvers import reverse

from marketcore.utils.strings import unique_slugify
from marketcore.models import City
from marketapps.directory.models import BusinessCategory, PaymentMethod, BusinessAspect
from marketapps.directory.models.managers import BusinessManager

class Business(models.Model):
    """A business in the Directory."""
    
    name = models.CharField(blank=False, max_length=64)
    slug = models.SlugField(blank=True, null=True)
    city = models.ForeignKey(City)
    categories = models.ManyToManyField(BusinessCategory)
    active = models.BooleanField(default=False)
    sponsored = models.BooleanField(default=False)
    date_added = models.DateField(blank=True, null=True, auto_now_add=True)
    date_updated = models.DateField(blank=True, null=True, auto_now=True)
    
    phone_number = models.CharField(blank=True, max_length=16)
    address_street = models.CharField(blank=False, max_length=64)
    address_city = models.CharField(blank=False, max_length=32)
    address_state = models.CharField(blank=False, max_length=4)
    address_zipcode = models.CharField(blank=False, max_length=10)
    longitude = models.FloatField(blank=True, null=True)
    latitude = models.FloatField(blank=True, null=True)
    business_email = models.EmailField(blank=True, null=True)
    show_open_status = models.BooleanField(default=True)
    
    website = models.URLField(blank=True, verify_exists=True)
    about = models.TextField(blank=True)
    payment_methods = models.ManyToManyField(PaymentMethod, blank=True, null=True)
    facebook_page = models.URLField(blank=True, verify_exists=True)
    aspects = models.ManyToManyField(BusinessAspect, blank=True, null=True, related_name="aspects")
    
    foreign_id = models.IntegerField(blank=True, null=True)
    
    # Managers
    objects = models.Manager()
    businesses = BusinessManager()
    
    class Meta:
        verbose_name_plural = "businesses"
        ordering = ['name']
        app_label = 'directory'
    
    def __unicode__(self):
        return self.name
    
    @property
    def is_categorized(self):
        return (self.categories.count() > 0)
    
    def get_absolute_url(self):
        """Returns the absolute URL to this business's profile page."""
        return reverse('directory-business-detail', args=[self.city.slug, self.slug])
    
    def hours_known(self):
        """Returns whether this business has a schedule entered."""
        if self.schedules.all():
            return True
        else: return False
    
    def is_open(self):
        """Returns whether the business is currently open."""
        now = datetime.datetime.now().time()
        day = datetime.datetime.now().weekday()
        days = self.schedules.get(name="Business Hours").days
        hours = days.get(day=day)
        
        # They are closed today.
        if not hours.time_open or not hours.time_close:
            return False
        
        # Always open.
        if hours.time_open == hours.time_close:
            return True
        
        # Standard hours
        if hours.time_open <= now < hours.time_close:
            return True

        # This doesn't necessarily mean they're not open, 
        # they may have an odd schedule        
        if hours.time_open > hours.time_close:
            if hours.time_open <= now:
                return True
        
        # See if perhaps this business is still open from yesterday
        if day == 0: hours = days.get(day=6)
        else: hours = days.get(day=day-1)
        
        if hours.time_open > hours.time_close:
            if hours.time_close > now:
                return True
        
        return False # all else fails, they are closed
    
    def next_opens_display(self):
        """Returns a dictionary of information about when this business opens next."""
        
        now = datetime.datetime.now().time()
        day = today = datetime.datetime.now().weekday()
        days = self.schedules.get(name="Business Hours").days
        hours = days.get(day=day)
        
        # if they've already opened today, try tomorrow
        if hours.time_open is None or hours.time_open < now:
            if day == 6: 
                day = 0
            else: day += 1
            hours = days.get(day=day)
        
        # seek the next day they are open
        while hours.time_open is None:
            if day == 6: 
                day = 0
            else: day += 1
            hours = days.get(day=day)
        
        if day == today:
            opens = "Opens today"
        elif day == (datetime.date.today() + datetime.timedelta(1)).weekday():
            opens = "Opens tomorrow"
        else:
            opens = "Opens %s" % (hours.DAY_CHOICES[day][1])
        
        return { 'opens': opens, 'time': hours.time_open }
    
    def next_closes_display(self):
        """Returns a dictionary with information about when this business next closes."""
        
        now = datetime.datetime.now().time()
        day = today = datetime.datetime.now().weekday()
        days = self.schedules.get(name="Business Hours").days
        hours = days.get(day=day)
        time = hours.time_close
        
        if time.hour > 17 or time.hour < 3:
            closes = "Closes tonight"
        else:
            closes = "Closes today"
        
        return { 'closes': closes, 'time': time }
    
    def set_category_ancestors(self):
        """
        Goes through the classified's categories and adds all 
        ancestor categories of its categories.
        """
        for cat in self.categories.exclude(parent=None):
            parent = cat.parent
            while parent != None:
                self.categories.add(parent)
                parent = parent.parent
    
    def save(self, *args, **kwargs):
        """Saves the business to the database."""
        
        if self.slug == "" or self.slug is None:
            self.slug = unique_slugify(self, self.name, Business.objects.filter(city=self.city))
        
        is_new = (None == self.id)
        
        if not self.longitude or not self.latitude:
            try:
                place, (lat, lng) = geocoders.Google().geocode("%s, %s, %s" % (self.address_street, self.address_city, self.address_state))
                self.longitude = lng
                self.latitude = lat
            except: pass
        
        super(Business, self).save(*args, **kwargs)
        if is_new or getattr(self, 'set_category', False):
            self.set_category_ancestors()

